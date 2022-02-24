#ifndef DEMO_SERVER_SOCK_H_
#define DEMO_SERVER_SOCK_H_

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace demo_server
{

struct Reply{
public:
    unsigned int reply_;
    long long time_;

    Reply():reply_(0),time_(0){};
};


class Sock{
public:
    explicit Sock() = default;
    Sock(const Sock& other):sock_fd_(other.sock_fd_),
                            sock_addr_(other.sock_addr_),
                            ip_(other.ip_),
                            port_(other.port_){
    };


    Sock(const int sock_fd, const struct sockaddr_in& addr):sock_fd_(sock_fd),
                                                            sock_addr_(addr),
                                                            ip_(inet_ntoa(addr.sin_addr)),
                                                            port_(ntohs(addr.sin_port)){
    };

    Sock& operator=(const Sock& other){
        this->sock_addr_ = other.sock_addr_;
        this->ip_ = other.ip_;
        this->port_ = other.port_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os,const Sock& sock){
        os << std::string("[") << std::string(sock.ip_) << std::string(",") << std::to_string(sock.port_) << std::string("]");
        return os;
    }

    inline int get_sock_fd(){
        return sock_fd_;
    }

    int send(const char*,unsigned int);

    int recv(char*,unsigned int);

private:
    struct sockaddr_in sock_addr_;
    int sock_fd_;
    char* ip_;
    unsigned int port_;
};
} // namespace name
#endif