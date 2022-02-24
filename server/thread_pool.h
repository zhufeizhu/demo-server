#ifndef DEMO_SERVER_THREADPOOL_H_
#define DEMO_SERVER_THREADPOOL_H_

#include <thread>
#include <mutex> 
#include <queue>
#include <vector>
#include <atomic>
#include <condition_variable>
#include "task.h"

namespace demo_server{

class Task;

class ThreadPool{
public:
    using Task_Ptr = std::unique_ptr<Task>;

    ThreadPool();

    ThreadPool(int num_threads);
    
    void shutdown();

    bool add_task(Task_Ptr);
    
    ~ThreadPool();
private:
    const int min_num_threads_ = 1;  //最小线程数
    const int max_num_threads_ = 20; //最大线程数
    int num_thread_;

    std::atomic_bool shutdown_;

    std::vector<std::thread> threads_;

    std::queue<Task_Ptr> tasks_;

    std::mutex task_mutex_;

    /*每个线程的主循环体*/
    void loop();
};

}
#endif