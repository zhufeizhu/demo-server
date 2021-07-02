#ifndef DEMO_THREAD_TASK_H
#define DEMO_THREAD_TASK_H

#include <functional>

namespace demo_server{

class ThreadTask{
    typedef std::function<void(void*)> Callback;
public:
    ThreadTask(Callback& callback, void* args);

    void Run();
private:
    Callback func_ ;
    void* args_;
};
}

#endif