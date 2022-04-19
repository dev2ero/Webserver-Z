#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <cassert>
#include <queue>

#include "condition.h"
#include "mutexlock.h"

using namespace std;

class ThreadPool {
public:
    enum ShutdownMode { GRACEFUL_QUIT, IMMEDIATE_SHUTDOWN };
    ThreadPool(size_t thread_num, ShutdownMode shutdown_mode = GRACEFUL_QUIT, size_t max_queue_size = -1);
    ~ThreadPool();
    bool append_task(void (*function)(void*), void* arguments);
private:
    static void* task_for_worker_threads_(void* arg);
    struct ThreadPoolTask {
        void (*function)(void*);
        void* arguments;
    };
    size_t thread_num_;
    size_t max_queue_size_;
    queue<ThreadPoolTask> task_queue_;
    vector<pthread_t> threads_;
    MutexLock threadpool_mutex_;
    Condition threadpool_cond_;
    ShutdownMode shutdown_mode_;
};

#endif
