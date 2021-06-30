#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>

#include "log.h"

pid_t threads[MAXEVENTS];

int StartServer(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);

    if(server_fd == -1){
        LOG(ERROR,"create socket_fd failed");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9990);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_fd,(struct sockaddr*)&addr,sizeof(addr))){
        LOG(ERROR,"bind addr failed");
        return -1;
    }

    if(listen(server_fd,10)){
        LOG(ERROR,"listen failed");
        return -1;
    }
    
    return server_fd;
}

/**
 * 初始化server 将server_fd加入到epoll中
 * @param max_size:epoll的最大监听数
 * @param server_fd:server的句柄
**/ 
int InitServer(int max_size, int server_fd){
    int epoll_fd = epoll_create(MAXEVENTS);

    if(epoll_fd == -1){
        LOG(ERROR,"epoll create failed");
        return -1;
    }

    struct epoll_event event;
    event.data.fd = server_fd;
    event.events = EPOLLIN;

    /*将server_fd添加到epoll的队列中 其中EPOLLIN表示监听其是否可读*/
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&event)){
        LOG(ERROR,"add server fd failed");
        return -1;
    }

    return epoll_fd;
}

/**
 * 将client_fd添加到epoll中
 * @param epoll_fd:epoll的句柄
 * @param client_fd:要添加的client句柄
 * @param flag:client的模式
**/ 
int AddEpoll(int epoll_fd,int client_fd,__uint32_t flag){
    struct epoll_event event;
    event.data.fd = client_fd;
    event.events = flag;

    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&event)){
        LOG(INFO,"add client fd to epoll failed");
        return -1;
    }
    
    return 0;
}

/**
 * 将client_fd从epoll中移除
**/ 
int DelEpoll(int epoll_fd, int client_fd){
    struct epoll_event event;
    event.data.fd = client_fd;
    
    if(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,client_fd,&event)){
        LOG(INFO,"delete client fd failed");
        return -1;
    }
    return 0;
}

void HandleRequest(struct epoll_event event){
    
}

void AcceptConnection(int epoll_fd, int server_fd){
    struct sockaddr_in client;
    bzero(client,sizeof(client));
    int client_len = 0;
    int accept_fd = 0;
    while(accept_fd = accept(server_fd,(struct sockaddr*)(&client),&client_len) >0){
        __uint32_t flag = EPOLLIN | EPOLLET | EPOLLONESHOT;//可读时触发 边缘触发 触发一次后从epoll中移除
        AddEpoll(epoll_fd,accept_fd,flag);
    }
}

void Loop(int epoll_fd, int server_fd){
    struct epoll_event events[MAXEVENTS];
    int num_fds = 0;
    for(;;){
        num_fds = epoll_wait(epoll_fd,events,MAXEVENTS,-1);
        for(int i = 0; i < num_fds; i++){
            if(num_fds < 3){
                LOG(ERROR,"epoll wait failed");
                return;
            }
            if(events[i].data.fd == server_fd){
                AcceptConnection(epoll_fd,server_fd);
            }else{
                HandleRequest(events[i]);
            }
        }
    }
}