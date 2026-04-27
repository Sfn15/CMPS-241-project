#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "config.h"

int runServer(int fd[2]){
    
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);

    //create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);


    // bind socket to IP/port
    memset(&address, 0, addr_len);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr*) &address, addr_len) < 0){
        perror("Bind failed :(\n   ");
        close(server_fd);
        return 1;
    }

    printf("Waiting for client \n");

    if (listen(server_fd, 1) < 0) {
    perror("listen failed");
    close(server_fd);
    return 1;
    }

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addr_len);
    
    printf("Client connected\n");

    const char* start = "START\n";
    write(client_fd, start, strlen(start));




    fd[0] = server_fd;
    fd[1] = client_fd;

    return 0;
}


    // int bytes;


    // //send & receive
    // while(1){
    //     int bytes = read(client_fd, buffer, BUFSIZE-1);
    //     if (bytes <= 0){
    //         break;
    //     }
        
    //     buffer[bytes]='\0';


    //     printf("Received: %s \n",buffer);

    //     //Game logic goes here


    //     const char* board = "BOOOOOOAAAARDD"; // print the actual board later

    //     write(client_fd, board, strlen(board));
    // }

    // close(client_fd);
    // close(server_fd);
    void* networkThread(void *ptr){
        SharedMove *moveInfo = (SharedMove *) ptr;

        int client_fd = moveInfo->client_fd;

        char buffer[BUFSIZE];
        int move[4];

        while(1){
            int bytes = read(client_fd, buffer, BUFSIZE-1);
            
            if(bytes <= 0){
                break;
            }

            buffer[bytes] = '\0';

            //printf(" Received: %s\n", buffer);

            //check for move validity
            if( sscanf(buffer, "%d %d %d %d", &move[0], &move[1], &move[2], &move[3]) != 4){
                write(client_fd, "INVALID\n",strlen("INVALID\n"));
                continue;
            }

            //entering critical section (writing to shared memory moveInfo)
            pthread_mutex_lock(&moveInfo->mutex);

            // if previous move is not done processing, ignore message

            if(moveInfo->ready == 1){
                pthread_mutex_unlock(&moveInfo->mutex);
                write(client_fd, "WAIT\n", strlen("WAIT\n"));
                continue;
            }

            for(int i =0; i < 4; i++){
                moveInfo->move[i] = move[i];
            }

            moveInfo->ready = 1;

            //exiting critical section
            pthread_mutex_unlock(&moveInfo->mutex);

        }

        return NULL;
    }