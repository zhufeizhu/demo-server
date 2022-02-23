#include "server.h"

#include <string.h>
#include <iostream>
#include <stdio.h>

#include "log.h"


namespace demo_server{

DemoServer::DemoServer(int max_size) noexcept{
    this->server_fd_ = socket(AF_INET,SOCK_STREAM,0);

    if(this->server_fd_ == -1){
        LOG(ERROR,"create socket_fd failed");
        return;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(this->server_fd_,(struct sockaddr*)&addr,sizeof(addr))){
        LOG(ERROR,"bind addr failed");
        return;
    }

    if(listen(this->server_fd_,max_size)){
        LOG(ERROR,"listen failed");
        return;
    }

    //poolPtr.reset(new ThreadPool(NUM_THREADS));
}

/**
 * 初始化server 将server_fd加入到epoll中
 * @param server_fd:server的句柄
**/ 
void DemoServer::init_server(){
    this->epoll_fd_ = epoll_create(DemoServer::MAXEVENTS);

    if(this->epoll_fd_ == -1){
        LOG(ERROR,"epoll create failed");
        return;
    }

    struct epoll_event event;
    event.data.fd = this->server_fd_;
    event.events = EPOLLIN | EPOLLET;

    /*将server_fd添加到epoll的队列中 其中EPOLLIN表示监听其是否可读*/
    if(epoll_ctl(this->epoll_fd_,EPOLL_CTL_ADD,this->server_fd_,&event)){
        LOG(ERROR,"add server fd failed");
        return;
    }
}

/**
 * 将client_fd添加到epoll中
 * @param epoll_fd:epoll的句柄
 * @param client_fd:要添加的client句柄
 * @param flag:client的模式
**/ 
int DemoServer::add_epoll(int client_fd,__uint32_t flag){
    struct epoll_event event;
    event.data.fd = client_fd;
    event.events = flag;

    if(epoll_ctl(this->epoll_fd_,EPOLL_CTL_ADD,client_fd,&event)){
        LOG(INFO,"add client fd to epoll failed");
        return -1;
    }else{
        LOG(INFO,"add client fd to epoll succeed");
    }
    
    return 0;
}

/**
 * 将client_fd从epoll中移除
**/ 
int DemoServer::del_epoll(int client_fd){
    struct epoll_event event;
    event.data.fd = client_fd;
    
    if(epoll_ctl(this->epoll_fd_,EPOLL_CTL_DEL,client_fd,&event)){
        LOG(INFO,"delete client fd failed");
        return -1;
    }
    return 0;
}

/*
    接收到请求后将收到的消息打印出来
*/
void DemoServer::handle_request(struct epoll_event event){
    // auto func= [](void* str){
    //     std::cout<<(char*)str<<std::endl;
    // };
    int len = 10;
    char msg[10];
    int n = 0;
    while(len > 0){
        n = recv(event.data.fd,msg + n,len,0);
        len -= n;
    }
    auto iter = connections_.find(event.data.fd);
    if(iter != connections_.end()){
        std::cout<<connections_[event.data.fd]<<"New Message is: "<<msg<<std::endl;
    }
    
    // ThreadTask* task = new ThreadTask(func,msg);
    // poolPtr->add_task(task);
}

void DemoServer::accept_connection(){
    struct sockaddr_in client;
    bzero((void*)&client,sizeof(client));
    unsigned int client_len = sizeof(client);
    int accept_fd = accept(this->server_fd_,(struct sockaddr*)&client,&client_len);
    if(accept_fd < 0){
        LOG(ERROR,"connect failed");
        return;
    }else{
        //getsockname(accept_fd,(struct sockaddr*)&client,&client_len);
        std::cout<<inet_ntoa(client.sin_addr)<<std::endl;
        std::cout<<ntohs(client.sin_port)<<std::endl;
        LOG(INFO,"connect suucceed");
    }
    __uint32_t flag = EPOLLIN | EPOLLET;//可读时触发 边缘触发 触发一次后从epoll中移除
    if(!add_epoll(accept_fd,flag)){
        connections_[accept_fd] = client;
    }
}

void DemoServer::close_connection(struct epoll_event event){
    std::cout<<"fd " << event.data.fd << " 的连接已经断开!"<<std::endl;
    if(!del_epoll(event.data.fd)){
        auto iter = connections_.find(event.data.fd);
        if( iter != connections_.end()){
            connections_.erase(iter);
        }
    }
}

void DemoServer::loop(){
    struct epoll_event events[MAXEVENTS];
    int num_fds = 0;
    for(;;){
        std::cout<<"Looping~"<<std::endl;
        num_fds = epoll_wait(this->epoll_fd_,events,MAXEVENTS,-1);
        std::cout<<"num fd is "<<num_fds<<std::endl;
        for(int i = 0; i < num_fds; i++){
            if(events[i].data.fd == this->server_fd_){
                accept_connection();
            }else if(events[i].events & EPOLLHUP){ //表示断开
                close_connection(events[i]);
            }
            else if(events[i].events & EPOLLIN) {
                std::cout<<"New Request comimg!"<<std::endl;
                handle_request(events[i]);
            }   
        }
    }
}
}


