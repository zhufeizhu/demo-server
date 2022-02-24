#include "server.h"

#include <iostream>

int main(){
    demo_server::DemoServer* server = new demo_server::DemoServer(10);
    std::cout<<"111"<<std::endl;
    server->init_server();
    server->loop();
}