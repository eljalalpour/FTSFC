#pragma once

#include <atomic>
#include <immintrin.h>

#define _MAX_XBEGIN_RETRY 2
#define _MAX_XABORT_RETRY 3
#define _ABORT_LOCK_BUSY  0xff
#define FREE              1
#define PAUSE             asm volatile("pause" ::: "memory")
#define ALIGNAS(x)        __attribute__((aligned(x)))

class ALIGNAS(64) elided_spin_lock {
public:
    elided_spin_lock(uint16_t xbegin_retry_max, uint16_t abort_retry_max) :
        _lock_val(FREE),
        _xbegin_retry_max(xbegin_retry_max),
        _abort_retry_max(abort_retry_max)
        {
        _lock_ptr = & _lock_val;
    }

    elided_spin_lock() : elided_spin_lock(_MAX_XBEGIN_RETRY, _MAX_XABORT_RETRY) { }

    inline bool is_free() {
        return *_lock_ptr == FREE;
    }

    inline void lock() noexcept {
        unsigned xbegin_retry = 0;
        unsigned xabort_retry = 0;
        unsigned status;

        while (xbegin_retry < _MAX_XBEGIN_RETRY) {
            // Start transaction
            if ((status = _xbegin()) == _XBEGIN_STARTED) {
                // Check lock and put into read-set
                if (is_free())
                    return; //Execute in transaction

                // Abort transaction as lock is busy
                _xabort (_ABORT_LOCK_BUSY);
            }
            // Transaction may not succeed on a retry
            if (!(status & _XABORT_RETRY)) {
                // There is no chance for a retry
                if (xabort_retry >= _MAX_XABORT_RETRY)
                    break;
                xabort_retry ++ ;
            }
            xbegin_retry ++;
        }//while

        // acquire the fallback lock
        while (__sync_sub_and_fetch(_lock_ptr, 1) < 0) {
            do
                PAUSE;
            while (!is_free());
            /* Could do respeculation here */
        }//while
    }

    inline void unlock() noexcept {
        if (is_free())
            _xend(); // Commit transaction else
        else
            *_lock_ptr = FREE;
    }

private:
    int _lock_val;
    int* _lock_ptr;

    uint16_t _xbegin_retry_max;
    uint16_t _abort_retry_max;
};

class ALIGNAS(64) flag_spin_lock {
public:
    flag_spin_lock() {
        _lock.clear();
    }

    inline void lock() noexcept {
        while (_lock.test_and_set(std::memory_order_acq_rel))
            ;
    }

    inline void unlock() noexcept {
        _lock.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _lock;
};

template <class T>
class elided_lock {
public:
    inline elided_lock(T& t) {
        _lock_ptr = &t;
        _lock_ptr->lock();
    }

    inline ~elided_lock() {
        _lock_ptr->unlock();
    }

private:
    T* _lock_ptr;
};

