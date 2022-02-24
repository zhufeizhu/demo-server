#ifndef DEMO_SERVER_TASK_H_
#define DEMO_SERVER_TASK_H_
#include <memory>

#include "sock.h"

namespace demo_server{

class Sock;

class Task{
public:
    Task() = default;
    virtual void run();
};

class FileTask: public Task{
public:
    FileTask(const std::string& file_path,std::shared_ptr<Sock> sock):file_path_(file_path),
                                                      sock_ptr_(sock){
    }

    FileTask(const FileTask& other):Task(){
        this->file_path_ = other.file_path_;
    }

    FileTask(FileTask&& other):Task(){
        this->file_path_ = std::move(other.file_path_);
    }

    FileTask& operator=(const FileTask& other);

    FileTask& operator=(FileTask&& other);

    bool operator==(const FileTask& other) const;

    void run() override;
private:
    std::string file_path_;
    std::shared_ptr<Sock> sock_ptr_;
    const static int data_file_t_= 1;

    struct FileMeta{
        uint8_t type_;
        unsigned int len_;
    };
};


}//namespace demo_server
#endif