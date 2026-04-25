#include "helper.h"
#include"config.h"
#include "display.h"
#include "bot.h"

#include <stdio.h>
#include <unistd.h>


int main(){
    char players[2] = {'A','B'};
    int scores[2]={0,0};
    int point[3]= {0};
    int botMove[4];
    int absoluteNonsense;

    char vlines[HEIGHT][LENGTH+1]={0};
    char hlines[HEIGHT+1][LENGTH]={0}; 
    char owned[HEIGHT][LENGTH]={0};
    int botGame = 0; // 0 for pvp, 1 for pv bot
    char yesNo;

 while (1) {
    printf("Play vs bot? [Y/n]: \n");
    scanf(" %c", &yesNo);

    if (yesNo == 'Y' || yesNo == 'y') {
        botGame = 1;  
        break;
    } else if (yesNo == 'N' || yesNo == 'n') {
        botGame = 0;
        break;
    } else {
        printf("Invalid input. Please enter Y/y or N/n.\n");
    }
}

    



    int i= 0;
if(botGame){

    while(scores[0] + scores[1] < HEIGHT*LENGTH){
        displayBoard(vlines,hlines, owned);

        if(i%2){
            absoluteNonsense = moveA(players[i%2],vlines, hlines, owned, botMove);

            printf("\n Bot's move: %d %d -> %d %d\n\n", botMove[0],botMove[1],botMove[2],botMove[3]);

           int v = validateInput(&botMove);
           point[0] = v;
           point[1] = botMove[0];
           point[2] = botMove[1];
           doMove(vlines, hlines, point);
        
            sleep(2); // if a bot captures a chain, it can be a bit disorienting, we 
            //sleep here to give the player time to take in their crushing defeat. 
        } else {
            while (playerMove(players[i%2], vlines, hlines, point)){
                continue;
            }   
        }

        int boxesClaimed = boxCheck(players[i%2], vlines, hlines, owned, point);
        scores[i%2] += boxesClaimed;
        if(boxesClaimed){
            printf("Updated score : A: %d, B: %d\n",scores[0],scores[1]);
        }
        
        if(boxesClaimed == 0) {
            i++;
        }
    }

} else {

     while(scores[0] + scores[1] < HEIGHT*LENGTH){ // while there are unclaimed squares
        displayBoard(vlines, hlines, owned);

        while (playerMove(players[i%2], vlines, hlines, point)){ // keep playing until a valid input is registered
            continue; 
        }
        
        int boxesClaimed = boxCheck(players[i%2], vlines, hlines, owned, point);
        scores[i%2] += boxesClaimed;
        if(boxesClaimed){
            printf("Updated score : A: %d, B: %d\n",scores[0],scores[1]);
        }
        
        if(boxesClaimed == 0) {
            i++;
        }
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
