#include "include/daemon.hpp"

#include <unistd.h>
#include <signal.h>

daemon::daemon(const std::string &path){
    this->config_path_ = path;
}

bool daemon::create(){
    pid_t pid = fork();

    if (pid < 0){
        perror("fork");
        return false;
    } else if (pid > 0) {
        /*父进程执行到这里直接退出*/
        exit(0);
    }

    if (setsid() < 0) {
        perror("setsid");
        exit(-1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(-1);
    } else if (pid > 0) {
        exit(0);
    }

    signal(SIGHUP,SIG_IGN);
    return true;
}