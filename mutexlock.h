#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <pthread.h>

class MutexLock {
    private:
        pthread_mutex_t mutex_;
    public:
        MutexLock()     { pthread_mutex_init(&mutex_, nullptr); }
        ~MutexLock()    { pthread_mutex_destroy(&mutex_); }
        void lock()     { pthread_mutex_lock(&mutex_); }
        void unlock()   { pthread_mutex_unlock(&mutex_); }
        pthread_mutex_t* get_mutex() { return &mutex_; }
};

class MutexLockGuard {
    private:
        MutexLock& lock_;
    public:
        MutexLockGuard(MutexLock& mutex) : lock_(mutex) { lock_.lock(); }
        ~MutexLockGuard() { lock_.unlock(); }
};

#endif 
