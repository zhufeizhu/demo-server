#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#ifndef PORT
#define PORT 9902
#endif

#ifndef MAX_LINE
#define MAX_LINE 40960
#endif

struct FileMeta{
    uint8_t type_;
    unsigned int len_;
};

struct Reply{
public:
    unsigned int reply_;
    long long time_;

    Reply():reply_(0),time_(0){};
};

int my_send(int sock_fd, const char* data, unsigned int len){
    int n = 0;
    int segment_size = 0;
    do{
        segment_size = (len-n > 4096)?4096:len-n;
    }
    while((n += ::send(sock_fd,data + n,segment_size,0)) < len);
    return n;
}

int my_recv(int sock_fd, char* data, unsigned int len){
    int n = 0;
    int segment_size = 0;
    do{
        segment_size = (len-n > 4096)?4096:len-n;
    }while((n += ::recv(sock_fd,data + n,segment_size,0)) < len);
    return n;
}


void str_cli(FILE *file,int sockfd){
    char sendline[MAX_LINE];

    while(fgets(sendline,MAX_LINE,file) != NULL) {
        if(sendline[0] == '0'){
            shutdown(sockfd,2);
        }
        
        if(send(sockfd,sendline,10,0) < 0){
            perror("send failed");
            return;
        } else {
            printf("send msg %s success\n",sendline);
        }

        struct FileMeta meta;
        my_recv(sockfd,reinterpret_cast<char*>(&meta),sizeof(meta));
        std::cout<<"File length is "<<meta.len_ <<std::endl;

        struct Reply reply;
        my_send(sockfd,reinterpret_cast<char*>(&reply),sizeof(reply));

        int len = meta.len_;
        int n = 0;
        int segment_size = 4096;
        char* buf = new char[4096]();
        do{
            segment_size = (len-n > 4096)?4096:len - n;
        }while((n += my_recv(sockfd,buf,segment_size)));
    }
}

int main(int argc, char* argv[]){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);

    if (!sockfd) {
        printf("creat socket failed");
        return 0;
    }


    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    /*将参数转换成numeric*/
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);

    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));

    if (ret) {
        perror("connect failed\n");
        return -1;
    }

    str_cli(stdin,sockfd);
}