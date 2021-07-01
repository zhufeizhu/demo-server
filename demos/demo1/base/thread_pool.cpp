#include "include/thread_pool.h"

namespace demo_server{

using namespace std;

ThreadPool::ThreadPool(){
    ThreadPool(this->min_num_threads_);
}

ThreadPool::ThreadPool(int num_thread){
    if(num_thread > this->max_num_threads){
        num_thread = this->max_num_threads;
    }

    this->num_thread_ = num_thread;
    for(int i = 0; i < this->num_thread_; i++){
        thread t(Loop);
        t.detach();
        this->threads_.push_back(t);
    }
}

bool ThreadPool::AddTask(ThreadTask* task){
    if(!task)   return false;
    unique_lock<mutex> lck(task_mutex_);
    if(this->tasks_.empty())    cond.notify_one();
    this->tasks_.push(task);
    return true;
}

void ThreadPool::Loop(){
    for(;;){
        unique_lock<mutex> lck(task_mutex_);
        if(this->tasks_.empty()) cond.wait(lck);
        
        ThreadTask* task = this->tasks_.front();
        this->tasks_.pop();
        task->Run();
    }
}


}