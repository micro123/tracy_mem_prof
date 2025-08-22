#include "spinlock.hpp"

void SpinLock::lock() noexcept
{
    int expected = 0;
    while(!__atomic_compare_exchange_n(&val_, &expected, 1, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        expected = 0;
    }
}

void SpinLock::unlock() noexcept
{
    int save = 0;
    __atomic_store(&val_, &save, __ATOMIC_RELEASE);
}

bool SpinLock::is_locked() noexcept
{
    return __atomic_load_n(&val_, __ATOMIC_ACQUIRE) != 0;
}


