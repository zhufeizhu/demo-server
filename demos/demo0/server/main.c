#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>


#ifndef PORT
#define PORT 9990
#endif

#ifndef MAX_LINE
#define MAX_LINE 4096
#endif

int server_start(){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        printf("creat server socket failed\n");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(server_socket,(struct sockaddr*)&addr,sizeof(addr));

    if (!ret) {
        printf("bind addr failed\n");
        return -1;
    }

    ret = listen(server_socket,10);

    if (!ret) {
        printf("listen socket failed\n");
        return -1;
    }

    return server_socket;
}

void handle(int confd,int servfd){
    char buf[MAX_LINE];
    int n;
again:
    while( (n = read(confd,buf,MAX_LINE)) > 0 ) {
        write(servfd,buf,n);
        /*EINTR表示发生了某个系统中断，此时表示因为某个系统中断导致了没有读取成功，因此需要重新读取*/
        if ((n < 0) && errno == EINTR) {
            goto again;
        } else if ( n < 0 ) {
            printf("read error\n");
            return 0;
        }
    }
}


void handle_connect(int servfd){
    while (1) {
        struct sockaddr_in client_addr;
        bzero(&client_addr,sizeof(client_addr));
        int client_size = sizeof(client_addr);
        /*等待连接*/
        int confd = accept(servfd,&client_addr,&client_size);

        int child_pid;

        if ( (child_pid = fork()) == 0 ) {
            /*这里子进程的server_socket引用减一*/
            close(servfd);
            handle(confd,servfd);
            exit(0);
        }
        close(confd);
    }
}



int main(){
    int servfd = server_start();

    handle_connect(servfd);

    return 0;
}