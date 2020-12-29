#ifndef _DEMO_SERVER_DAEMON_HPP_
#define _DEMO_SERVER_DAEMON_HPP_

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