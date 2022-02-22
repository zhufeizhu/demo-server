#ifndef DEMO_SERVER_H
#define DEMO_SERVER_H

namespace demo_server{

class T1{
public:
    T1(T1&& other) = delete;  //delete move constructor
    T1& operator=(T1&& other) = delete;   //delete move assignment
};

}

#endif