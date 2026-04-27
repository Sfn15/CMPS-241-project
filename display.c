#include "display.h"
#include "config.h"
#include <string.h>
#include <stdio.h>

void displayBoard(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH] , char owned[HEIGHT][LENGTH]){
    printf("  0 1 2 3 4 5                  dots and boxes by Stephane and Hassan\n");
    char line [2*LENGTH + 1];
    for(int i = 0; i < HEIGHT + 1 ; i++){
        printf("%d ",i);// print line index

        line[2*LENGTH] = '.'; // set the last character as a dot because we always have a dot
        for(int j = 0; j < 2*LENGTH ; j++ ){ // prints dots along with horizontal lines
             if(j%2){ // for odd j, we write lines
                if(hlines[i][(j-1)/2] != '\0'){ // if there is a line then add it
                    line[j] = hlines[i][(j-1)/2];
                } else {
                    line[j] = ' ';
                }
             } else { // for even j, write dots
                line[j] = '.';
             }
        }
        printf("%s\n",line);

        if( i != HEIGHT){ // prevents owned and vlines from going out of bounds, they are shorter than hlines
            for(int j = 0; j < 2*LENGTH + 1; j++){//  print characters along with vert lines
                if(j%2){ //same idea as last loop
                    if(owned[i][(j-1)/2] != '\0'){
                        line[j] = owned[i][(j-1)/2];
                    } else {
                        line[j]= ' ';
                    }
                } else {
                    if(vlines[i][j/2] != '\0'){
                        line[j] = vlines[i][j/2];
                    } else {
                        line[j] = ' ';
                    }
                }
            }
            printf("  %s\n",line);
        }
       
    }
}

void onlineDisplay(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH], char buffer[BUFSIZE]){
    size_t pos = 0; 
    memset(buffer, '\0',strlen(buffer));
}
    