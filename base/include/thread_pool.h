#ifndef DEMO_THREAD_H
#define DEMO_THREAD_H

#include <thread>
#include <mutex> 
#include <queue>
#include <vector>
#include <atomic>
#include <condition_variable>

#include "thread_task.h"

namespace demo_server{

using namespace std;

class ThreadPool{
public:
    ThreadPool();
    ThreadPool(int num_threads);
    ThreadPool(ThreadPool& pool) = delete;//不支持拷贝构造
    ThreadPool& operator=(const ThreadPool&) = delete;//不支持赋值运算符
    void Shutdown();
    /*向任务队列添加任务*/
    bool AddTask(ThreadTask* task);
    ~ThreadPool();
private:
    const int min_num_threads_ = 5;
    const int max_num_threads = 20;
    condition_variable cond;
    vector<thread> threads_;
    int num_thread_;
    atomic<bool> shutdown_;
    mutex thread_mutex_;
    mutex task_mutex_;
    queue<ThreadTask*> tasks_;

    /*每个线程的主循环体*/
    void Loop();
};

}

#endif