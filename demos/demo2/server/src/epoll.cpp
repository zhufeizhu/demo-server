#include "include/epoll.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Epoll::Epoll(int port, int maxlen)
{
    this->listen_port_ = port;
    this->maxlen_ = maxlen;
}

bool Epoll::init(int len)
{
    int listen_socket = socket(AF_INET,SOCK_STREAM,0);
    if (listen_socket < 0) {
        return;
    }
    this->socket_fd_ = listen_socket;
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(get_listen_port());
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(listen_socket,(struct sockaddr*)&addr,sizeof(addr));
    if (ret < 0) {
        return false;
    }

    int maxlen = get_maxlen();

    if (maxlen <= 0){
        return false;
    }  

    ret = listen(listen_socket,maxlen);
    if (ret < 0) {
        return false;
    }

    int epoll_fd = epoll_create(len);
    if (epoll_fd < 0) {
        return false;
    }

    this->epoll_fd_ = epoll_fd;

    struct epoll_event event;
    event.data.fd = listen_socket;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_socket,&event) == -1) {
        return false;
    }
}

void Epoll::epoll_add(int client_fd)
{
    struct epoll_event event;
    event.data.fd = client_fd;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,client_fd,&event) == -1) {
        _exit(-1);
    }
}

void Epoll::epoll_mod(int client_fd)
{
    return;
}


void Epoll::getRequest()
{
    int nepoll = epoll_wait(epoll_fd_,events,max_epoll_,-1);

    for (int i = 0; i < nepoll; i++) {
        
        if (this->events[i].data.fd == this->socket_fd_) {
            /*表明当前是socked_fd有数据可读*/
            addRequest();
        } else {

        }
    }
}

void Epoll::add