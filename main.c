#include "helper.h"
#include"config.h"
#include "display.h"
#include <stdio.h>


int main(){
    char vlines[HEIGHT][LENGTH+1]={0};
    char hlines[HEIGHT+1][LENGTH]={0}; 
    char owned[HEIGHT][LENGTH]={0};

    vlines[2][3]='|';
    vlines[2][4]='|';
    owned[2][3]='A';
    hlines[2][2] ='_';
    hlines[3][2] = '_';
   

    displayBoard(vlines, hlines, owned);

    return 0;
}