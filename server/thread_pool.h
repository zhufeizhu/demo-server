// #ifndef DEMO_THREAD_H
// #define DEMO_THREAD_H

// #include <thread>
// #include <mutex> 
// #include <queue>
// #include <vector>
// #include <atomic>
// #include <condition_variable>

// #include "thread_task.h"
// #include "noncopyable.h"
// #include "nonmoveable.h"

// namespace demo_server{

// class ThreadPool : public T1, public T2 {
// public:
//     ThreadPool();

//     ThreadPool(int num_threads);
    
//     void shutdown();

//     bool add_task(ThreadTask* task);
    
//     ~ThreadPool();
// private:
//     const int min_num_threads_ = 5;
//     const int max_num_threads = 20;
//     std::condition_variable cond;
//     int num_thread_;

//     std::vector<std::thread> threads_;
//     std::atomic<bool> shutdown_;
//     std::mutex thread_mutex_;
//     std::mutex task_mutex_;
//     std::queue<ThreadTask*> tasks_;

//     /*每个线程的主循环体*/
//     void loop();
// };

// }

// #endif