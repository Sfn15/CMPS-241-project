#ifndef HELPER_H
#define HELPER_H

#define BUFSIZE 4096

#include "config.h"

int boxCheck (char p, char vlines[HEIGHT][LENGTH+1],char hlines[HEIGHT+1][LENGTH],char owned[HEIGHT][LENGTH], int coord[3]); 
//return 1 if a player now claimed a square
//if so, edit the owned array
//coord[0] = 1 if the new line is vertical and 0 otherwise

int playerMove(char p, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int coord[3]);

int onlineMove(int move[4] ,char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int coord[3]);

int doMoveOnline(char vlines[HEIGHT][LENGTH + 1], char hlines[HEIGHT + 1][LENGTH], int coord[3]);
#endif

//char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH]