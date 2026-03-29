#include "helper.h"
#include"config.h"
#include "display.h"
#include <stdio.h>


int main(){
    char players[2] = {'A','B'};
    int scores[2]={0,0};
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
        
        int boxesClaimed = boxCheck(players[i%2], vlines, hlines, owned, point);
        scores[i%2] += boxesClaimed;
        //printf("Updated score : A: %d, B: %d\n",scores[0],scores[1]);
        
        if(boxesClaimed == 0) {
            i++;
        }
   }

   if(scores[0] > scores[1]){
    printf("Player %c wins with a score of %d!",players[0],scores[0]);
   } else if (scores[0] == scores[1]){
    printf("The game is a tie with both players having a score of %d", (HEIGHT*LENGTH)/2);
   } else {
    printf("Player %c wins with a score of %d!",players[1],scores[1]);
   }





    return 0;
}
