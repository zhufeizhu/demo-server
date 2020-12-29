#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

#ifndef PORT
#define PORT 9989
#endif

#ifndef MAXLINE
#define MAXLINE 1024
#endif

void dg_cli(FILE *fd, int sockfd, struct sockaddr_in *addr, socklen_t len){
    char sendline[MAXLINE],revcline[MAXLINE];
    int n;

    while((n = fgets(sendline,MAXLINE,fd)) < 0) {
        sendto(sockfd,sendline,n,0,addr,len);
    }
}

int main(){
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.s_addr);

    dg_cli(stdin,sockfd,&addr,sizeof(addr));

}