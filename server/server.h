#ifndef DEMO_SERVER_SERVER_H
#define DEMO_SERVER_SERVER_H

#include <sys/epoll.h>
#include <iostream>

#include <memory>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sock.h"
#include "log.h"
#include "thread_pool.h"

namespace demo_server{

class DemoServer{
public:
    static const int PORT = 9902;
    static const int MAXEVENTS = 20;
    static const int NUM_THREADS = 5;
    DemoServer(int max_size) noexcept;
    void init_server();
    void loop();
private:
    int epoll_fd_;
    int server_fd_;
    int max_connection_;    //支持的最大连接数
    const char* root_path_;

    std::unordered_map<int,std::shared_ptr<Sock>> connections_;   //创建的fd与对应的addr的map 
    std::unique_ptr<ThreadPool> pool_ptr_;    //线程池

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