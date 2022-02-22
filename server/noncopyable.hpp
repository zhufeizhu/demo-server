#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

namespace demo_server{
class NonCopyable{
protected:    
    NonCopyable(const NonCopyable& other) = delete; //delete copy constructor
    NonCopyable& operator=(const NonCopyable& other) = delete; //delete copy assignment
    NonCopyable() = default;
    ~NonCopyable() = default;
};
}

#endif