#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <sys/epoll.h>








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

    if(bind(server_socket,(struct sockaddr*)&addr,sizeof(addr))){
        printf("bind addr failed\n");
        return -1;
    }

    if(listen(server_socket,10)){
        printf("listen socket failed\n");
        return -1;
    }

    return server_socket;
}

int server_init(int maxsize, int server_fd){
    int epoll_fd = epoll_create(maxsize);
    
    if (epoll_fd == -1) {
        perror("epoll_create");
        return -1;
    }

    struct epoll_event event;
    event.data.fd = server_fd;
    event.events = EPOLLIN;

    int ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&event);

    if (ret == -1) {
        perror("epoll_ctl:");
        return -1;
    }

    return epoll_fd;
}

void epoll_add(int epoll_fd,int new_fd,__uint32_t FLAG){
    struct epoll_event new_event;
    new_event.data.fd = new_fd;
    new_event.events = FLAG;

    if (epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_fd,&new_event) == -1) {
        perror("epoll_add");
        return -1;
    }
    return;
}

void epoll_del(int epoll_fd, int old_fd){
    struct epoll_event old_event;
    old_event.data.fd = old_fd;
    if (epoll_ctl(epoll_fd,EPOLL_CTL_DEL,old_fd,&old_event)) {
        perror("epoll_del");
        return -1;
    }
}

void acceptConnection(int epoll_fd, int server_fd){
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr));
    socklen_t client_addr_len = 0;
    int accept_fd = 0;
    while((accept_fd = accept(server_fd,(struct sockaddr*)&client_addr,&client_addr_len)) > 0 ){
        /*将获取到的新的连接注册到epoll的监听事件中*/
        __uint32_t flag = EPOLLIN | EPOLLET | EPOLLONESHOT;
        epoll_add(epoll_fd,accept_fd,flag);
    }
}

void readRequest(void *param_fd){
    int fd = (int)param_fd;
    

}

void handleRequest(struct epoll_event event){
    pthread_t pid;
    int fd = event.data.fd;
    if (pthread_create(&pid,NULL,readRequest,(void*)fd)) {

    }
}

void getRequest(int epoll_fd, int server_fd ){
    struct epoll_event wait_events[MAXEVENTS];

    int event_count = epoll_wait(epoll_fd,wait_events,MAXEVENTS,-1);

    if (event_count < 0){
        perror("epoll_wait");
        return -1;
    }

    for (int i = 0; i < event_count; i++) {
        int fd = wait_events[i].data.fd;
        
        if (fd < 3) {
            perror("getRequest");
            return -1;
        } else if (fd == server_fd) {
            acceptConnection(epoll_fd,server_fd);
        } else {
            handleRequest(wait_events[i]);
        }
    }
}


void handle_connect(int servfd){
    for(;;) {
        struct sockaddr_in client_addr;
        bzero(&client_addr,sizeof(client_addr));
        int client_size = sizeof(client_addr);
        /*等待连接*/
        
    }
}

int main(){
    int servfd = server_start();

    handle_connect(servfd);

    return 0;
}