#ifndef DEMO_SERVER_LOG_H_
#define DEMO_SERVER_LOG_H_

#include <stdio.h>

#define ERROR "[ERROR]"
#define INFO "[INFO]"
#define DEBUG "[DEBUG]"

inline void LOG(const char* tag, const char* msg){
#ifdef LOG_ON
    //TODO
#else
    printf("%s:%s\n",tag,msg);
#endif
}

#endif