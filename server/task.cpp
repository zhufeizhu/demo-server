#include "task.h"

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

namespace demo_server{

void Task::run(){
};

bool FileTask::operator==(const FileTask& other) const{
    if(this->file_path_ == other.file_path_ && this->sock_ptr_ == other.sock_ptr_){
        return true;
    }else{
        return false;
    }
}

FileTask& FileTask::operator=(const FileTask& other){
    this->file_path_  = other.file_path_;
    return *this;
}

FileTask& FileTask::operator=(FileTask&& other){
    if(other == *this){
        return *this;
    }else{
        this->file_path_ = std::move(other.file_path_);
        return *this;
    }
}


void FileTask::run(){
    const char* file_path = file_path_.c_str();
    if(!access(file_path,F_OK|R_OK)){
        int fd = open(file_path,O_RDONLY);
        if(fd){
            std::cout<<"文件打开失败"<<std::endl;
            return;
        }else{
            struct stat buf;
            if(::stat(file_path,&buf)){
                std::cout<<"获取文件信息失败"<<std::endl;
            }

            struct FileMeta meta;
            meta.len_ = buf.st_size;
            meta.type_ = data_file_t_;
            sock_ptr_->send(reinterpret_cast<char*>(&meta),sizeof(meta));

            struct Reply reply;
            sock_ptr_->recv(reinterpret_cast<char*>(&reply),sizeof(reply));
            if(reply.reply_ == 0){
                int n = 0;
                char* buf = new char[4096]();
                while((n = read(fd,reinterpret_cast<void*>(buf),4096)) > 0){
                    sock_ptr_->send(buf,n);
                }
            }
        }
    }else{
        std::cout<<"文件"<<this->file_path_<<"不存在"<<std::endl;
    }
}
}