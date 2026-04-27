#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define BUFSIZE 4096

int main(){
    const char* hostname = "127.0.0.1";
    const char* port = "8080";

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(hostname, port, &hints, &res) != 0){
        perror("getaddrinfo failed\n");
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0){
        perror("socket creation failed!\n");
        return 1;
    }

    
    
    if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0){
        perror("Connection failed :(\n");
        close(sockfd);
        return 1;
    }

    freeaddrinfo(res);

    printf("Connected to server\n");

    char buffer[BUFSIZE];
    int input[4];

    while(1){
        int bytes = read(sockfd, buffer, BUFSIZE -1);
        if(bytes <= 0){
            break;
        }

        buffer[bytes] = '\0';

        printf("Board : %s\n", buffer);

        for(int i = 0; i < 4; i++){
            scanf("%d", &input[i]);
        }

        char message[64];
        snprintf(message, sizeof(message), "Move: %d %d %d %d\n", input[0], input[1], input[2], input[3]);
        write(sockfd, message, strlen(message));
        // have the server check for validity later, now this is good enough
    }

    close(sockfd);
    return 0;



}