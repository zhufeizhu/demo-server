#include "server.h"

int main(){
    demo_server::DemoServer demo(10);
    demo.init_server();
    demo.loop();
}