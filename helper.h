#ifndef HELPER_H
#define HELPER_H

#include "config.h"

void boxCheck (char p, char vlines[HEIGHT][LENGTH+1],char hlines[HEIGHT+1][LENGTH],char* owned[HEIGHT][LENGTH], int* scores[2], int* square[2],int coord[3]); 
//return an array indicating which box was owned
//return -1 in the first element or null pointer if no square was given
//fills in the coordinate in square[2]
//coord[0] will tell you if it is a vertical or horizontal line, the other two are coordinates for where the line was made

void playerMove(char p, char* vlines[HEIGHT][LENGTH+1], char* hlines[HEIGHT+1][LENGTH]);

#endif

//char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH]