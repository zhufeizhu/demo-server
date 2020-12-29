# 关于I/O的一些事情

以套接字的输入操作为例，I/O可以分成两步    
**进程调用read操作**  
    1. 数据从网络传输到内核  
    2. 数据从内核拷贝到进程的缓冲区    
**read操作返回并读到了需要的数据**

## 同步I/O
**导致请求进程阻塞，直到I/O操作完成**
* 阻塞I/O  
  从read开始,进程一直在等待read结束，除非被其他信号中断  
* 非阻塞I/O
  从read开始,在阶段1中不停的询问内核数据是否传输完毕，传输完毕后在阶段2阻塞进程并最后读取数据  
* 信号驱动I/O
  从read开始直接返回，在阶段1并不阻塞进程，当数据传输完成后通知进程可以执行阶段2 
* I/O复用
  将read操作分成了两步，分别是select和read，即等待数据到达内核表明可读以及从内核拷贝并读取数据

## 异步I/O
**不导致请求进程阻塞**
* 异步I/O  
  从read开始调用时进程就可以做其他事情，当整个读取动作完成之后系统通知进程read操作完成了

*tip:异步I/O和信号驱动I/O 的区别在于前者是通知I/O操作何时完成，后者是通知何时开始I/O操作*

## I/O复用

### `select`函数
*允许进程指示内核等待多个事件中的任何一个发生，并仅在由一个或多个事件发生或经历一段事件后才唤醒它*
```
    #include <sys/select.h>  
    #include <sys/time.h>  

    int select(int maxfdp1,fdset *readset, fdset *writeset, fd_set *exceptset, const struct timeval *timeout);
```
1. 其中最后一个参数timeout可以由如下三种情况
* 当为NULL时表示仅有一个描述字准备好I/O时才返回
* 当为0时表示检查完描述字之后立即返回
* 当为某个值时表示在等待的时间内有一个描述字准备好返回

2. fd_set数据类型表示的是整数数组，其中每个整数中的每一位对应一个描述字，系统提供了如下的函数来对fd_set进行操作
```
    void FD_ZERO(fd_set *fdset); //清空fdset的所有位
    void FD_SET(int fd, fd_set *fdset); //将fd对应的bit位置1
    void FD_CLEAR(int fd, fd_set *fdset); //将fd对应的bit位置0
    int FD_ISSET(int fd, fd_set *fdset); //检测fd对应的bit位是否为1
```
*举个例子:*
```
    fd_set fset;  
    FD_ZERO(&fset);  //先置空,很重要!!
    FD_SET(5,&fset);  //将fd=5置为1
    FD_CLEAR(5,&fset);  //将fd=5置为0
    FD_ISSET(5,&fset);  //检测fd=5,发现为0
```
如果我们对read write或者except中的某个fd_set不感兴趣的话，直接传NULL即可
3. `maxfdp1`表示待测试的描述字个数，值等于**待测试的最大描述字+1**(因此这个maxfdp1表示的意思是*maxfd plus 1* ^-^)  
4. `select`函数的返回值表示所有描述字集的已就绪的总位数，如果超时则返回0

### `epoll`函数
*与`select`函数功能类似，用于监听某个事件的发生*
```
    #include <sys/epoll.h>  

    int epoll_create(int size);
    int epoll_ctl(int epfd, int op, struct epoll_event *event);
    int epoll_wait(int epfd,struct epoll_event *events, int maxevents, int timeout);
```

* `epoll_create`: 生成一个epoll专用的文件描述符,**因此当不再使用时需要调用`close`关闭该描述符**,其中size可以填大于0的任何值
    
* `epoll_ctl`: epoll的事件注册函数，用于向系统注册需要监听的事件类型
- `epfd`: epoll对应的文件描述符,即`epoll_create`的返回值
- `op`: 表示注册动作，分别有三种
  1. `EPOLL_CTL_ADD`: 注册新的fd到epfd中
  2. `EPOLL_CTL_MOD`: 修改已经注册的fd的监听事件
  3. `EPOLL_CTL_DEL`: 从epfd中删除指定的fd
- `fd`: 要监听的文件描述符fd
- `event`: 注册的fd需要监听的事件,其为`struct epoll_event`类型:
```
    typedef union epoll_data{
        void *ptr;
        int fd;
        __uint32_t u32;
        __uint64_t u64;
    } epoll_data_t;

    struct epoll_event{
        __uint32_t events;
        epoll_data_t data; 
    };
```
其中`events`可以为如下几种类型：
1. `EPOLLIN`:表示对应的文件描述符可以读
2. `EPOLLOUT`:表示对应的文件描述符可以写
3. `EPOLLPRI`:表示对应的文件描述符由紧急数据可读
4. `EPOLLERR`:表示对应的文件描述符出错
5. `EPOLLHUP`:表示对应的文件描述符被挂起
6. `EPOLLET`:表示设置为ET模式
7. `EPOLLONESHOT`:表示仅监听一次  
   
成功返回0 失败返回-1

* `epoll_wait`:等待事件的产生，如果有监听的事件产生则返回需要处理的事件的个数
- `epfd`:epoll对应的文件描述符,即`epoll_create`的返回值
- `events`: 保存发生事件的events数组
- `maxevents`: 需要监听的最大events的个数
- `timeout`: 设置的等待超时时间，如果为-1表示没有上限

举个栗子

## 套接口选项
**用来获取和设置影响套接口的选项**
```
  #include <sys/socket.h>  
  
  int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
  int setsockopt(int sockfd, int level, int optname, const void *optva, socklen_t *optlen);
```

其中`setsockpot`是将`optval`的值写入到`sockfd`对应的套接口中,`getsockopt`是读取`sockfd`对应的值到`optval`中


