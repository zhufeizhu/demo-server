#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <sys/epoll.h>
#include <iostream>

#include <memory>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "noncopyable.hpp"
//#include "thread_pool.h"
//#include "thread_task.h"

namespace demo_server{

class Sock{
public:
    explicit Sock() = default;
    Sock(const Sock& other):ip_(other.ip_),port_(other.port_){};
    Sock(const struct sockaddr_in& addr):ip_(inet_ntoa(addr.sin_addr)),port_(ntohs(addr.sin_port)){};
    Sock& operator=(const Sock& other){
        this->ip_ = other.ip_;
        this->port_ = other.port_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os,const Sock& sock){
        os << std::string("[") << std::string(sock.ip_) << std::string(",") << std::to_string(sock.port_) << std::string("]");
        return os;
    }
private:
    char* ip_;
    unsigned int port_;
};

class DemoServer{
public:
    static const int PORT = 9901;
    static const int MAXEVENTS = 20;
    static const int NUM_THREADS = 5;
    DemoServer(int max_size) noexcept;
    void init_server();
    void loop();
private:
    int epoll_fd_;
    int server_fd_;
    int max_connection_;    //支持的最大连接数

    std::unordered_map<int,Sock> connections_;   //创建的fd与对应的addr的map 
    //std::unique_ptr<ThreadPool> poolPtr;    //线程池

    int add_epoll(int client_fd,__uint32_t);

    int del_epoll(int client_fd);

    void accept_connection();

    void read_request();

    void handle_request(struct epoll_event event);

    void get_request();

    void handle_connection();

    void close_connection(struct epoll_event event);
};
}

#endif