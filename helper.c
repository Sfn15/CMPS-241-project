#include "helper.h"
#include <stdio.h>

void playerMove(char p, char* vlines[HEIGHT][LENGTH+1], char* hlines[HEIGHT+1][LENGTH]){
    int move[4];

    printf("Player %c's turn. Enter the row and column of the first dot and second dot:\n",p);
    for(int i = 0; i < 4; i++){
        scanf("%d", &move[i]);
    }
}