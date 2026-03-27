#ifndef HELPER_H
#define HELPER_H

#include "config.h"

void boxCheck (int move[4], int lines[HEIGHT][LENGTH], char owned[HEIGHT][LENGTH]);

void playerMove(int move[4], char p, int* scores[2]);

#endif