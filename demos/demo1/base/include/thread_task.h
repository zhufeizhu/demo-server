#ifndef DEMO_THREAD_TASK_H
#define DEMO_THREAD_TASK_H

#include <functional>

namespace demo_server{

class ThreadTask{
    typedef function<void(void*)> callback;
public:
    ThreadTask();
    void Run();
private:
    callback func_ ;
};
}

#endif