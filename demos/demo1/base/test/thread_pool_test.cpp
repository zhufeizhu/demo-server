#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <memory>

#include "../include/thread_pool.h"
#include "../include/thread_task.h"

// class ThreadPoolTest: public testing::Test{
// protected:
//     void SetUp() {
//         pool = new demo_server::ThreadPool(5);
//     };

//     void TearDown() {
//         delete pool;
//     }
//     demo_server::ThreadPool* pool;
// };



// TEST_F(ThreadPoolTest,run){
//     int a = 10;
//     std::function<void(void*)> f = [](void* num){
//         (*(int*)num)++;
//     };
//     demo_server::ThreadTask* task = new demo_server::ThreadTask(f,(void*)&a);
//     pool->AddTask(task);
//     EXPECT_EQ(a,11);
//     pool->Shutdown();
// }


int main(int argc, char** argv){
    // ::testing::InitGoogleTest(&argc,argv);
    // return RUN_ALL_TESTS();
    std::unique_ptr<demo_server::ThreadPool> poolPtr(new demo_server::ThreadPool(3));
    int a = 10;
    std::function<void(void*)> f = [](void* num){
        sleep(3);
        std::cout<<*(int*)(num)<<"  "<<std::this_thread::get_id()<<std::endl;
    };
    for(int i = 0; i < 10; i++){
        demo_server::ThreadTask* task = new demo_server::ThreadTask(f,(void*)&i);
        poolPtr->AddTask(task);
    }
}
