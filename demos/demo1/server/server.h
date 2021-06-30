#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

#include <sys/epoll.h>

#ifndef PORT
#define PORT 9990
#endif

#ifndef MAX_LINE
#define MAX_LINE 4096
#endif

#ifndef MAXEVENTS 
#define MAXEVENTS 20
#endif

int StartServer();

int InitServer(int max_size, int server_fd);

int AddEpoll(int epoll_fd,int client_fd,__uint32_t);

int DelEpoll(int epoll_fd, int client_fd);

void AcceptConnection();

void ReadRequest();

void HandleRequest(struct epoll_event event);

void GetRequest();

void HandleConnect();

#endif