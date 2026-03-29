#include "helper.h"
#include <stdio.h>

int validateInput(int (*move)[4]){
    // return -1 for invalid input
    // return 0 for horizontal line
    // return 1 for vertical line
    // also order the input so that the logic is easier
    int *m = *move;
    int temp;

    for(int i = 0; i < 2; i++){ //check for out of bounds input
        if(m[2*i] > HEIGHT || m[2*i] < 0){
            return -1;
        }
        if(m[2*i+1]> LENGTH || m[2*i+1] < 0){
            return -1;
        }
    }
    if(m[0]==m[2]){ // if the input is horizontal
        if(m[1]-m[3] == 1 || m[3] - m[1] == 1){ //check the others are exactly one apart
            if(m[1] > m[3]){
                temp = m[3];
                m[3] = m[1];
                m[1] = temp;
            }
            return 0;
        } else {
            return -1;
        }
    } else if (m[1] == m[3]){ //if the input is vertical
        if(m[0]-m[2] == 1 || m[2] - m[0] == 1){ //check the others are exactly one apart
            if(m[0] > m[2]){
                temp = m[2];
                m[2] = m[0];
                m[0] = temp;
            }
            return 1;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}


// return 1 if failed and 0 if successful.
int playerMove(char p, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int coord[3]){
    int move[4];

    printf("Player %c's turn. Enter the row and column of the first dot and second dot:\n",p);
    int n;
    // need to 1. Make sure input is valid here and 
    // 2. if input is invalid, scanf will not flush it automatically. So we need to flush it manually:
    for(int i = 0; i < 4; i++){
        n = scanf("%d", &move[i]);
        if ( n != 1){
            printf("Invalid input, please try again\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            return 1;
            //this will keep reading forward until the buffer is empty
            // since getchar() and scanf both use stdin, it will get flushed
        }
    }
    //now make sure input is valid
    int v = validateInput(&move);

    if(v == -1){
        printf("Invalid input, please try again\n");
        return 1;
    }

    coord[0] = v;
    coord[1] = move[0];
    coord[2] = move[1];

    if(v){    
        if (vlines[move[0]][move[1]] == '\0'){ //check a line does not already occupy this space
            vlines[move[0]][move[1]] = '|';
            return 0;
        } else {
            printf("There is already a line here, please try again.\n");
            return 1;
        }
    } else {    
        if (hlines[move[0]][move[1]] == '\0'){ //check a line does not already occupy this space
            hlines[move[0]][move[1]] = '-';
            return 0;
        } else {
            printf("There is already a line here, please try again.\n");
            return 1;
        }
    }
}

int boxCheck (char p, char vlines[HEIGHT][LENGTH+1],char hlines[HEIGHT+1][LENGTH],char owned[HEIGHT][LENGTH], int coord[3]) {
    int boxesFilled = 0;
    int r = coord[1];
    int c = coord[2];
    
    if (coord[0] == 1) {    //LINE IS VERTICAL
        //check box to the left
        if(c>0) { //if not on leftmost edge
            if(vlines[r][c-1] != '\0' && hlines[r][c-1] != '\0' && hlines[r+1][c-1] != '\0') {
                owned[r][c-1] = p;
                boxesFilled++;
            }
        }
        //check box to the right
        if(c < LENGHT ) { //if not on rightmost edge
            if(vlines[r][c+1] != '\0' && hlines[r][c] != '\0' && hlines[r+1][c] != '\0') {
                owned[r][c] = p;
                boxesFilled++;
            }
        }
    }
    
    else {    //LINE IS HORIZONTAL
        //check box above
        if(r>0) { //if not on top edge
            if(hlines[r-1][c] != '\0' && vlines[r-1][c] != '\0' && vlines[r-1][c+1] != '\0') {
                owned[r-1][c] = p;
                boxesFilled++;
            }
        }
        //check box below
        if(r < HEIGHT ) { //if not on bottom edge
            if(hlines[r+1][c] != '\0' && vlines[r][c] != '\0' && vlines[r][c+1] != '\0') {
                owned[r][c] = p;
                boxesFilled++;
            }
        }
    }

    return boxesFilled;
}
