#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#define BUFSIZE 4096

int main(){
    char hostname[128];
    printf("Enter server IP (default is 127.0.0.1): ");
    fgets(hostname, sizeof(hostname), stdin);

    if(hostname[0] == '\n'){
        strcpy(hostname, "127.0.0.1");
    } else {
        hostname[strcspn(hostname, "\n")] = '\0';
    }

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

    char acc[BUFSIZE *2] = {0};
    int acc_len = 0;

    int inBoard = 0;
    int shouldInput = 0;
    int isMyTurn = 0;

    while(1){

        int bytes = read(sockfd, buffer, BUFSIZE -1);
        if(bytes <= 0){
            break;
        }

        buffer[bytes] = '\0';

        //printf("Raw read: %s\n",buffer);


        strcat(acc, buffer);

        char *line_start = acc;
        char *newline;

        /*
        A major issue with the previous loop is that if several messages are received
        at once, everything breaks (bad). This ensures that the buffer is fully
        'consumed' before moving on to reading the next thing. 

        the loop condition basically checks for a newline characters, where strchr != NULL
        whenever one is found, meaning that there is still a line to read,
        since the server and main are designed to only send messages that terminate with
        \n.

        newline points to the \n character and so it separates every message received
        */
        while((newline = strchr(line_start, '\n')) != NULL){
            *newline = '\0'; 

            char *line = line_start;

            if(strcmp(line, "CLIENT TURN") == 0){
                isMyTurn = 1;
                line_start = newline + 1;
                continue;
            }

            if(strcmp(line, "SERVER TURN") == 0){
                isMyTurn = 0;
                line_start = newline + 1;
                continue;
            }

            if (strcmp(line, "BEGIN BOARD") == 0){
                inBoard = 1;
                line_start = newline +1;
                continue;
            }

            if(strcmp(line, "END BOARD") == 0){
                inBoard = 0;

                if(isMyTurn){
                printf("Player B's turn. Enter the row and column of the first dot and second dot:\n");
                shouldInput = 1;
                } else {
                    printf("Waiting for player A's move... \n");
                }
                line_start = newline + 1;
                break;
            }

            if(inBoard){
                printf("%s\n",line);
                line_start = newline + 1;
                continue;
            }

            if (strcmp(line, "INVALID") == 0){
                printf("Invalid input, please try again\n");
            } else if (strcmp(line, "OCCUPIED") == 0){
                printf("There is already a line here, please try again\n");
            } else if (strcmp(line, "WAIT") == 0){
                // do nothing, you just have to wait 
            }  else {
                printf("FALLBACK CASE ! %s\n",line);
                line_start = newline +1;
                continue;
            }

            

            line_start = newline +1;
        }


        //moves the unread part of acc back to the beginning
        memmove(acc, line_start, strlen(line_start) + 1);

    
        if(!shouldInput){
            continue;
        }

        shouldInput = 0;
       
        for(int i = 0; i < 4; i++){
            scanf("%d", &input[i]);
        }

        char message[64];

        //printf("ASKING FOR INPUT\n");
        snprintf(message, sizeof(message), "%d %d %d %d\n", input[0], input[1], input[2], input[3]);
        write(sockfd, message, strlen(message));
        //printf("MOVE SENT\n");
        // have the server check for validity later, now this is good enough
    }

    close(sockfd);
    return 0;



}