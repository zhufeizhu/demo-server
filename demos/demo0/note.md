# 基本TCP套接口编程

## 基本流程
1. 服务端启动并等待客户端链接
2. 客户端启动并向服务端发送请求
3. 服务端收到请求进行处理后进行应答
4. 客户端收到应答后关闭
5. 服务器知道客户端关闭后也关闭
   
## 关键函数

1. socket函数:指定通信的类型

    #include <sys/socket.h>
    
    int socket(int family,int type,int protocol);

2. conntect函数:服务端建立与TCP服务器的连接

    #include <sys/socket.h>

    int connect(int sockfd, const struct sockaddr *serveraddr, socklen_t addrlen);

该函数会激发TCP的三次握手，当且仅当建立成功或者出错时会返回
当connect失败后,需要关闭当前的close并重新调用socket

3. bind函数:把本地协议地址赋予一个套接口

    #include <sys/socket.h>

    int bind(int sockfd,const struct sockaddr *myaddr, socklen_t addrlen);

实际上就是设定了ip和端口号

4. listen函数:监听连接的客户端

    #include <sys/socket.h>

    int listen(int sockfd,int backlog);

5. accept函数:从已完成连接队列的头部返回下一个已完成连接，如果队列为空则睡眠

    #include <sys/socket.h>

    int accept(int sockfd, struct sockaddr *client_addr,socklen_t *addrlen);

其中client_addr和addrlen表示返回的已连接的客户的协议地址

该函数涉及到了两个套接口,一个是sockfd所代表的被动连接的一端，称之为监听套接口,一个是client_addr所代表的主动连接的一段

6. readn、writen和readline函数

    #include <unp.h>

    ssize_t readn(int filedes. void *buffer, size_t nbytes);
    
    ssize_t writen(int filedes, void *buffer, size_t nbytes);

    ssize_t readline(int filedes, void *buffer, size_t nbytes);

上述函数与以往的read和write函数不同，通过read和write函数读取到的数据可能比应该读到的要少，因为socket中的缓存已经到达了极限，需要重复调用read或write才可以输入或者输出剩余的字节，而上述函数就是为了解决这个问题。

以readn为例

    #include "unp.h"

    ssize_t readn(int fd, void *vptr, size_t n){
        size_t nleft;   //剩余要读的字节数
        size_t nread;   //已经读的字节数
        char *ptr;      //read的缓冲区

        ptr = vptr;
        nleft = n;
        while (nleft > 0) { //判断是否已经读完，未读完的话重复读
            if ((nread = read(fd,ptr,nleft)) < 0 ) {
                if (errno = EINTR)
                    nread = 0;
                else
                    return -1;
            } else if (nread == 0) {
                break;
            }

            nleft  -= nread;
            ptr += nread;
        }
        return (n-nleft);
    }

7. close函数：关闭套接口

    #include <unistd.h>

    int close(int sockfd);

该函数就是文件系统的close函数，具有引用计数，如果真的向直接关闭TCP连接的话可以直接调用shutdown