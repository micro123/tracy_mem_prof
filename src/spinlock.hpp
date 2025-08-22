#ifndef SPINLOCK_HPP
#define SPINLOCK_HPP

struct SpinLock {
    
    void lock() noexcept;

    void unlock() noexcept;

    bool is_locked() noexcept;

    int val_{0};
};

struct LockGuard {
    LockGuard(SpinLock &l): sl(l) { l.lock(); }
    ~LockGuard() { sl.unlock(); }

    SpinLock &sl;
};

#endif // SPINLOCK_HPP
