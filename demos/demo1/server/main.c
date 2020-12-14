#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>


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
    addr.sin_port = htons(9990);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(server_socket,(struct sockaddr*)&addr,sizeof(addr));

    if (ret) {
        perror("bind addr failed\n");
        return -1;
    }

    ret = listen(server_socket,10);

    if (ret) {
        printf("listen socket failed\n");
        return -1;
    }

    return server_socket;
}

void handle(int confd,int servfd){
    char buf[MAX_LINE];
    int n;
again:
    while((n = recv(confd,buf,MAX_LINE,0)) > 0 ) {
        printf("revc msg is %s",buf);
        /*EINTR表示发生了某个系统中断，此时表示因为某个系统中断导致了没有读取成功，因此需要重新读取*/

        if (send(confd,buf,n,0) > 0) {
            printf("send msg success");
        } else {
            printf("send msg failed");
        }
    }
}


void handle_connect(int servfd){
    for(;;) {
        struct sockaddr_in client_addr;
        bzero(&client_addr,sizeof(client_addr));
        int client_size = sizeof(client_addr);
        /*等待连接*/
        int confd = accept(servfd,&client_addr,&client_size);

        if (confd < 0){
            return -1;
        }

        pthread_t pid;

        int ret = pthread_create(&pid,NULL,&handle,);
        if (ret < 0) {
            printf("create thread failed\n");
            return -1;
        }
    }
}



int main(){
    int servfd = server_start();

    handle_connect(servfd);

    return 0;
}