#include "display.h"
#include"config.h"
#include <stdio.h>

void displayBoard(int vlines[HEIGHT-1][LENGTH+1], int hlines[HEIGHT][LENGTH+1] , char owned[HEIGHT][LENGTH]){
    printf("  0 1 2 3 4 5\n");
    for(int i = 0; i < HEIGHT + 1 ; i++){
        printf("%d ",i);
        for(int j = 0; j < LENGTH + 1 ; j++ ){
             printf(".");
        }
        printf("\n\n");
        
       
    }

    //TODO complete this guy
}