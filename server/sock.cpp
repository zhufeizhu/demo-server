#include "sock.h"
#include <sys/socket.h>

namespace demo_server{

int Sock::send(const char* data,unsigned int len){
    int n = 0;
    int segment_size = 0;
    do{
        segment_size = (len-n > 4096)?4096:len-n;
    }
    while((n += ::send(this->sock_fd_,data + n,segment_size,0)) < len);
    return n;
}

int Sock::recv(char* data,unsigned int len){
    int n = 0;
    int segment_size = 4;
    do{
        segment_size = (len-n > 4096)?4096:len-n;
    }while((n += ::recv(this->sock_fd_,data + n,segment_size,0)) < len);
}

}