#include "thread_pool.h"

int InitThreadPool(__uint8_t num_threads);

void AddTask();

void DestroyThreadPool();

void* RunThread(void*);