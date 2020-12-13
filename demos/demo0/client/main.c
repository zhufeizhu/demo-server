#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>

#ifndef PORT
#define PORT 9990
#endif

#ifndef MAX_LINE
#define MAX_LINE 4096
#endif

void str_cli(FILE *file,int sockfd){
    char sendline[MAX_LINE],recvline[MAX_LINE];

    while(fgets(sendline,MAX_LINE,file) != NULL) {
        write(sockfd,sendline,sizeof(sendline));
        if(read(sockfd,recvline,MAX_LINE) == 0) {
            printf("read error\n");
            return 0;
        }
        fputs(recvline,stdout);
    }
    return 0;
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
    addr.sin_port = htonl(PORT);
    
    /*将参数转换成numeric*/
    inet_pton(AF_INET,argv[1],&addr.sin_addr.s_addr);

    connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));

    str_cli(stdin,sockfd);
}