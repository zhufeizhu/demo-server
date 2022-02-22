#ifndef DEMO_LOG_H
#define DEMO_LOG_H

#include <stdio.h>

#define ERROR "[ERROR]"
#define INFO "[INFO]"
#define DEBUG "[DEBUG]"

void LOG(const char* tag, const char* msg){
#ifdef LOG_ON
    //TODO
#else
    printf("%s:%s\n",tag,msg);
#endif
}
#endif