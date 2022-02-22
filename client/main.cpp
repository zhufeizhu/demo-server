#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#ifndef PORT
#define PORT 9901
#endif

#ifndef MAX_LINE
#define MAX_LINE 4096
#endif

void str_cli(FILE *file,int sockfd){
    char sendline[MAX_LINE],recvline[MAX_LINE];

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