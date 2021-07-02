#include "include/server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

#include "base/include/log.h"


namespace demo_server{

DemoServer::DemoServer(int max_size){
    this->server_fd_ = socket(AF_INET,SOCK_STREAM,0);

    if(this->server_fd_ == -1){
        LOG(ERROR,"create socket_fd failed");
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9990);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(this->server_fd_,(struct sockaddr*)&addr,sizeof(addr))){
        LOG(ERROR,"bind addr failed");
        return;
    }

    if(listen(this->server_fd_,max_size)){
        LOG(ERROR,"listen failed");
        return;
    }

    poolPtr.reset(new ThreadPool(NUM_THREADS));
}

/**
 * 初始化server 将server_fd加入到epoll中
 * @param server_fd:server的句柄
**/ 
void DemoServer::InitServer(){
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
int DemoServer::AddEpoll(int client_fd,__uint32_t flag){
    struct epoll_event event;
    event.data.fd = client_fd;
    event.events = flag;

    if(epoll_ctl(this->epoll_fd_,EPOLL_CTL_ADD,client_fd,&event)){
        LOG(INFO,"add client fd to epoll failed");
        return -1;
    }
    
    return 0;
}

/**
 * 将client_fd从epoll中移除
**/ 
int DemoServer::DelEpoll(int client_fd){
    struct epoll_event event;
    event.data.fd = client_fd;
    
    if(epoll_ctl(this->epoll_fd_,EPOLL_CTL_DEL,client_fd,&event)){
        LOG(INFO,"delete client fd failed");
        return -1;
    }
    return 0;
}

void DemoServer::HandleRequest(struct epoll_event event){
    function<void(void*)> func= [](void* str){
        std::cout<<(char*)str<<std::endl;
    };
    char msg[200];
    recv(event.data.fd,msg,200,0);
    ThreadTask* task = new ThreadTask(func,msg);
    poolPtr->AddTask(task);
}

void DemoServer::AcceptConnection(){
    LOG(INFO,"connect suucceed");
    struct sockaddr_in client;
    bzero((void*)&client,sizeof(client));
    unsigned int client_len = 0;
    int accept_fd = 0;
    while(accept_fd = accept(this->server_fd_,(struct sockaddr*)(&client),&client_len) >0){
        __uint32_t flag = EPOLLIN | EPOLLET | EPOLLONESHOT;//可读时触发 边缘触发 触发一次后从epoll中移除
        AddEpoll(accept_fd,flag);
    }
}

void DemoServer::Loop(){
    struct epoll_event events[MAXEVENTS];
    int num_fds = 0;
    for(;;){
        num_fds = epoll_wait(this->epoll_fd_,events,MAXEVENTS,-1);
        for(int i = 0; i < num_fds; i++){
            if(events[i].data.fd == this->server_fd_){
                AcceptConnection();
            }else if(events[i].events & EPOLLIN){
                HandleRequest(events[i]);
            }
        }
    }
}
}


