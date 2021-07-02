#ifndef DEMO_THREAD_H
#define DEMO_THREAD_H

#include <thread>~

class ThreadPool{
public:
    static ThreadPool& getThreadPoolInstance();
private:
    std::thread
}


typedef struct{
    void* (*func)(void*);
    void* args;
}thread_task_t;

typedef struct{
    pthread_t* threads;
    __uint8_t min_num_threads;
    __uint8_t max_num_threads;
    __uint8_t num_thread;
    pthread_mutex_t *mutex;
    thread_task_t* task_queue;
    int queue_head;
    int queue_tail;
    int queue_max_size;
}thread_pool_t;

int InitThreadPool(__uint8_t num_threads);

void AddTask();

void DestroyThreadPool();

void* RunThread(void*);

#endif