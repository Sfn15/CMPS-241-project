#include "helper.h"
#include"config.h"
#include "display.h"
#include <stdio.h>


int main(){
    char players[2] = {'A','B'};
    int scores[2];
    int point[3]= {0};
    char vlines[HEIGHT][LENGTH+1]={0};
    char hlines[HEIGHT+1][LENGTH]={0}; 
    char owned[HEIGHT][LENGTH]={0};
    

    int i= 0;
    while(scores[0] + scores[1] < HEIGHT*LENGTH){ // while there are unclaimed squares
        displayBoard(vlines, hlines, owned);
        while (playerMove(players[i%2], vlines, hlines, point)){ // keep playing until a valid input is registered
            continue; 
        }
        //Need to add: if a player completes a square, repeat their turn


        i++; //incement to move to the next player
   }





    return 0;
}