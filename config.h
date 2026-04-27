#ifndef CONFIG_H
#define CONFIG_H

#include <pthread.h>

#define LENGTH 5
#define HEIGHT 4
#define BUFSIZE 4096

typedef struct {
    int move[4];
    int ready;
    pthread_mutex_t mutex;
    int client_fd;
}SharedMove;

#endif