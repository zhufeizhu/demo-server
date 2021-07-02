#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <sys/epoll.h>

#include <memory>

#include "base/include/thread_pool.h"
#include "base/include/thread_task.h"

namespace demo_server{
    
class DemoServer{

public:
    static const int PORT = 9900;
    static const int MAXEVENTS = 20;
    static const int NUM_THREADS = 5;
    DemoServer(int max_size);
    void InitServer();
    void Loop();
private:
    int epoll_fd_;
    int server_fd_;
    unique_ptr<ThreadPool> poolPtr;

    int AddEpoll(int client_fd,__uint32_t);

    int DelEpoll(int client_fd);

    void AcceptConnection();

    void ReadRequest();

    void HandleRequest(struct epoll_event event);

    void GetRequest();

    void HandleConnect();
};
}



#endif