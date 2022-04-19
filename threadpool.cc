#include "threadpool.h"

ThreadPool::ThreadPool(size_t thread_num, ShutdownMode shutdown_mode, size_t max_queue_size) 
    : thread_num_(thread_num),
    max_queue_size_(max_queue_size),
    threadpool_cond_(threadpool_mutex_),
    shutdown_mode_(shutdown_mode) {
    while(threads_.size() < thread_num_) {
        pthread_t thread;
        if(!pthread_create(&thread, nullptr, task_for_worker_threads_, this))
            threads_.push_back(thread);
    }
}

ThreadPool::~ThreadPool() {
    {
        MutexLockGuard guard(threadpool_mutex_);
        if(shutdown_mode_ == IMMEDIATE_SHUTDOWN)
            while(!task_queue_.empty())
                task_queue_.pop();
        auto pthread_exit_func = [] (void*) { pthread_exit(0); };
        ThreadPoolTask task = { pthread_exit_func, nullptr };
        for(size_t i = 0; i < thread_num_; i++)
            task_queue_.push(task);
        threadpool_cond_.notify_all();
    }
    for(size_t i = 0; i < thread_num_; i++)
        pthread_join(threads_[i], nullptr);
}

bool ThreadPool::append_task(void (*function)(void*), void* arguments) {
    MutexLockGuard guard(threadpool_mutex_);
    if(task_queue_.size() > max_queue_size_)
        return false;
    else {
        ThreadPoolTask task = { function, arguments };
        task_queue_.push(task);
        threadpool_cond_.notify();
        return true;
    }
}

void* ThreadPool::task_for_worker_threads_(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    ThreadPoolTask task;
    for(;;) {
        {
            MutexLockGuard guard(pool->threadpool_mutex_);
            while(pool->task_queue_.size() == 0)
                pool->threadpool_cond_.wait();
            assert(pool->task_queue_.size() != 0);
            task = pool->task_queue_.front();
            pool->task_queue_.pop();
        }
        (task.function)(task.arguments);
    }
    assert(0 && "NEVER REACH HRER");
    return nullptr;
}
