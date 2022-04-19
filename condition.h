#ifndef CONDITION_H
#define CONDITION_H

#include <cerrno>
#include <pthread.h>

#include "mutexlock.h"

class Condition {
    private:
        MutexLock& lock_;
        pthread_cond_t cond_;
    public:
        Condition(MutexLock& mutex) : lock_(mutex) { pthread_cond_init(&cond_, nullptr); }
        ~Condition()        { pthread_cond_destroy(&cond_); }
        void notify()       { pthread_cond_signal(&cond_); }
        void notify_all()   { pthread_cond_broadcast(&cond_); }
        void wait()         { pthread_cond_wait(&cond_, lock_.get_mutex()); }
        bool wait_for_seconds(size_t sec) {
            timespec abstime;
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += (time_t)sec;
            return ETIMEDOUT != pthread_cond_timedwait(&cond_, lock_.get_mutex(), &abstime);
        }
};

#endif
