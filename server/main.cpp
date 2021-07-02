#include "server/include/server.h"

int main(){
    demo_server::DemoServer demo(10);
    demo.InitServer();
    demo.Loop();
}