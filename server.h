#ifndef SERVER_H
#define SERVER_H
#include "config.h"

int runServer(int fd[2]);


void* networkThread(void *ptr);

#endif