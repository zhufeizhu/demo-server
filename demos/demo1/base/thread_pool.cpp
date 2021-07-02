#include "include/thread_pool.h"

#include <iostream>
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
        this->threads_.push_back(thread(&ThreadPool::Loop,this));
    }

    shutdown_.store(false);
}

bool ThreadPool::AddTask(ThreadTask* task){
    if(!task)   return false;
    unique_lock<mutex> lck(task_mutex_);
    if(this->tasks_.empty()) {
        this->tasks_.push(task);
        cond.notify_one();
    }else{
        this->tasks_.push(task);
    }
    return true;
}

void ThreadPool::Shutdown(){
    this->shutdown_.store(true);
}

void ThreadPool::Loop(){
    std::cout<<"thread "<<std::this_thread::get_id()<<" start"<<std::endl;
    for(;;){
        ThreadTask* task;
        {
            unique_lock<mutex> lck(task_mutex_);
            while(!shutdown_ && this->tasks_.empty()) cond.wait(lck);
            
            task = this->tasks_.front();
            this->tasks_.pop();
        }
        task->Run();
    }
}

ThreadPool::~ThreadPool(){
    for(int i = 0; i < this->num_thread_; i++){
        if(this->threads_[i].joinable()){
            this->threads_[i].join();
        }
    }
}
}