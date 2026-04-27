#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"

void displayBoard(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH]);

void onlineDisplay(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH], char buffer[BUFSIZE]);

#endif