#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <memory>

#include "../include/thread_pool.h"
#include "../include/thread_task.h"

class ThreadPoolTest: public testing::Test{
protected:
    void SetUp() {
        
    };

    void TearDown() {
        
    }
    
};

TEST_F(ThreadPoolTest,run){
    int num = 0;
    {
        std::unique_ptr<demo_server::ThreadPool> poolPtr(new demo_server::ThreadPool(3));
        std::function<void(void*)> f = [](void* num){
            (*(int*)num)++;
            std::cout<<*(int*)num<<std::endl;
            sleep(3);
        };
        
        for(int i = 0; i < 10; i++){
            demo_server::ThreadTask* task = new demo_server::ThreadTask(f,(void*)&num);
            poolPtr->AddTask(task);
        }
        sleep(20);
        poolPtr->Shutdown();
    }
    EXPECT_EQ(num,10);
    
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
