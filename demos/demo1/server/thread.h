#ifndef DEMO_THREAD_H
#define DEMO_THREAD_H

#include <pthread.h>

typedef struct{
    void* func(void*);
    void* args;
}thread_task_t;

typedef struct{
    pid_t* threads;
    __uint8_t num_thread;
    pthread_mutex_t *mutex;
    thread_task_t* tasks;
}thread_pool_t;

int InitThreadPool(__uint8_t num_threads);

void AddTask();

void DestroyThreadPool();

void* RunThread(void*){
    
}

#endif