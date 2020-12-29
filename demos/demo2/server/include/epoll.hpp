#ifndef _DEMO_SERVER_EPOLL_HPP_
#define _DEMO_SERVER_EPOLL_HPP_

// #include "type.hpp"
#include <sys/epoll.h>

#ifndef MAX_EPOLL //支持的最大监听的文件描述符个数 
#define MAX_EPOLL 100
#endif

class Epoll{    
    public:
        explicit Epoll(int port,int maxlen);
        bool init(int len);
        void epoll_add(int client_fd);
        void epoll_del(int client_fd);
        void epoll_mod(int client_fd);
        void getRequest();
        void addRequest();

        inline int get_epoll_fd(){
            return this->epoll_fd_;
        };

        inline int get_listen_port(){
            return this->listen_port_;
        };

        inline int get_maxlen(){
            return this->maxlen_;
        }

    private:
        int socket_fd_;
        int epoll_fd_;
        int listen_port_;
        int maxlen_;
        int max_epoll_ = MAX_EPOLL;
        struct epoll_event events[MAX_EPOLL];
};
#endif