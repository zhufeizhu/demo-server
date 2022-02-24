#include "thread_pool.h"

#include "sock.h"
#include "log.h"

namespace demo_server{

ThreadPool::ThreadPool(){
    ThreadPool(this->min_num_threads_);
}

ThreadPool::ThreadPool(int num_thread){
    LOG(INFO,"init thread popl");
    if(num_thread > this->max_num_threads_){
        num_thread = this->max_num_threads_;
    }

    shutdown_.store(false);
    this->num_thread_ = num_thread;
    this->tasks_ = {};
    for(int i = 0; i < this->num_thread_; i++){
        this->threads_.emplace_back(std::thread(&ThreadPool::loop,this));
    }
    
    LOG(INFO,"init thread pool succeed");
}

bool ThreadPool::add_task(Task_Ptr task){
    if(task == nullptr)   return false;
    std::unique_lock<std::mutex> lck(task_mutex_);
    if(this->tasks_.empty()) {
        this->tasks_.push(std::move(task));
        // cond.notify_one();
    }else{
        this->tasks_.push(std::move(task));
    }
    return true;
}

void ThreadPool::shutdown(){
    this->shutdown_.store(true);
    //cond.notify_all();//通知所有线程线程池关闭
}

void ThreadPool::loop(){
    std::cout<<"start looping"<<std::endl;
    while(!shutdown_){
        Task_Ptr task;
        {
            std::unique_lock<std::mutex> lck(task_mutex_);
            //cond.wait(lck,[this]{return this->shutdown_.load() || !this->tasks_.empty();});
            
            if(!shutdown_ && !this->tasks_.empty()){
                task = std::move(this->tasks_.front());
                this->tasks_.pop();
            }
        }
        if(task) task->run();
    }
}

ThreadPool::~ThreadPool(){
    std::cout<<"destructor"<<std::endl;
    shutdown_.store(true);
    for(int i = 0; i < this->num_thread_; i++){
        if(this->threads_[i].joinable()){
            this->threads_[i].join();
        }
    }
}
}