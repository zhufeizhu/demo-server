#include <sys/socket.h>
#include <arpa/inet.h>


#ifndef PORT
#define PORT 9989
#endif

#ifndef MAXLINE
#define MAXLINE 1024
#endif

void dg_echo(int server_fd, struct sockaddr_in *client_addr, socklen_t clilen){
    char msg[MAXLINE];
    for(;;) {
       int n = recvfrom(server_fd,msg,MAXLINE,0,client_addr,&clilen);
       printf("msg is %s\n",msg);
    }
}


int main(){
    int listen_fd = socket(AF_INET,SOCK_DGRAM,0);

    if (listen_fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr,client_addr;

    bzero(&addr,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));

    if (ret < 0) {
        perror("bind");
        return -1;
    }

    dg_echo(listen_fd,&client_addr,sizeof(addr));
}