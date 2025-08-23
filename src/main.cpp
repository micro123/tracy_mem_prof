#include <new>

#include <dlfcn.h>
#include <malloc.h>
#include <tracy/Tracy.hpp>

#define EXPORT __attribute__((visibility("default")))

namespace
{
    void* (*libc_malloc)(size_t) = nullptr;

    void* (*libc_calloc)(size_t,size_t) = nullptr;

    void* (*libc_realloc)(void*,size_t) = nullptr;

    void* (*libc_memalign)(size_t,size_t) = nullptr;

    void  (*libc_free)(void*) = nullptr;
}

#define TRACY_CTX(X) do { ++level; X; --level; } while(0)

static void load_sym() {
    *(void**)&libc_malloc = dlsym(RTLD_NEXT, "__libc_malloc");
    *(void**)&libc_calloc = dlsym(RTLD_NEXT, "__libc_calloc");
    *(void**)&libc_realloc = dlsym(RTLD_NEXT, "__libc_realloc");
    *(void**)&libc_memalign = dlsym(RTLD_NEXT, "__libc_memalign");
    *(void**)&libc_free = dlsym(RTLD_NEXT, "__libc_free");
}

thread_local int level = 0;

extern "C" {
    EXPORT
    int memory_profiler_enabled = 1;

    EXPORT
    void free(void* ptr) {
        if (__glibc_likely(level == 0))
            TRACY_CTX(TracyFree(ptr));
        libc_free(ptr);
    }

    EXPORT
    void *malloc(size_t sz) {
        if (libc_malloc == nullptr) {
            load_sym();
        }
        void *r = libc_malloc(sz);
        if (__glibc_likely(level == 0))
            TRACY_CTX(TracyAlloc(r, sz));
        return r;
    }

    EXPORT
    void *realloc(void *ptr, size_t sz)
    {
        if (libc_malloc == nullptr) {
            load_sym();
        }
        void *r = libc_realloc(ptr, sz);
        if (__glibc_likely(level == 0)) {
            TRACY_CTX(TracyFree(ptr);TracyAlloc(r, sz));
        }
        return r;
    }

    EXPORT
    void *memalign(size_t alignment, size_t sz)
    {
        if (libc_malloc == nullptr) {
            load_sym();
        }
        void *r = libc_memalign(alignment, sz);
        if (__glibc_likely(level == 0)) {
            TRACY_CTX(TracyAlloc(r, sz));
        }
        return r;
    }

    EXPORT
    void *calloc(size_t n, size_t sz)
    {
        if (libc_malloc == nullptr) {
            load_sym();
        }
        size_t total = n * sz;
        void *r = libc_calloc(n, sz);
        if (__glibc_likely(level == 0))
            TRACY_CTX(TracyAlloc(r, total));
        return r;
    }
}

void* operator new(size_t sz) { return malloc(sz); }
void* operator new[](size_t sz) { return malloc(sz); }
void operator delete(void *ptr) noexcept { free(ptr); }
void operator delete[](void *ptr) noexcept { free(ptr); }
