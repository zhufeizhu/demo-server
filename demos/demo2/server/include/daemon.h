#ifndef _DEMO_SERVER_DAEMON_HPP_
#define _DEMO_SERVER_DAEMON_HPP_

//暂时不考虑守护进程的问题

#include <string>

class daemon{
    public:
        daemon();
        daemon(const daemon &daemon);
        daemon(const std::string &path);
        bool create();
    private:
        std::string config_path_;
};

#endif