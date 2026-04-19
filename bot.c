#include "helper.h"
#include "bot.h"
#include <malloc.h>

int moveA(char p, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH], int coord[3]){
    int[4] move = {0, 0, 0, 0};

    struct chain chains[HEIGHT*LENGTH];
    
}

int countOpenSides(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH],int r, int c){
    int sides = 4;
    if(hlines[r][c]!='\0'){ // up
        sides--;
    } 
    if(hlines[r+1][c] != '\0'){ // down
        sides--;
    }
    if(vlines[r][c] != '\0'){ // left
        sides--;
    }
    if(vlines[r][c+1] != '\0'){ // right
        sides--;
    }
    return sides;
}

void getOpenDirections(int r, int c,char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int dirs[4]){
    
    dirs[0] = (hlines[r][c] == '\0'); //up
    dirs[1] = (hlines[r+1][c] == '\0'); // down
    dirs[2] = (vlines[r][c] == '\0'); // left
    dirs[3] = (vlines[r][c+1] == '\0'); // right

}


void findChains(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH],struct chain chains[HEIGHT*LENGTH]){

    int chainNum = 1;

    int chainArr[HEIGHT][LENGTH] = {0};
    int dirs[4];
    // explore the boxes array (check for owned boxes and ignore em) 
    for(int i = 0; i < HEIGHT; i++){

        for(int j = 0; j < LENGTH; j++){
           if(owned[i][j] != '\0' || chainArr[i][j]){ // if the box is owned or part of a chain ignore it
                continue;
            } 

            int d = countOpenSides(vlines, hlines, i, j); //if we have open sides
            
            if(d == 4 || d== 3 || d == 0){ 
                continue;
            }

            int endpointCount = 0;
            int nDegree, cDegree;

            int cr = i, cc = j; // cr: current row, cc: current col
            int pr = -1, pc = -1; // previous row previous col 
            // pr pc is to make sure we are not backtracking

            int endpoint = -1;
            int chainLength = 0;

            while(1){
                chainArr[cr][cc] = chainNum;
                chains[chainNum].blocks[chainLength] = cr*LENGTH + cc;
                //chainArr[cr][cc] = chainNum;
                chainLength++;

                cDegree = countOpenSides(vlines, hlines, cr, cc);

                if(cDegree == 1){
                    endpointCount++;
                    if(endpoint == -1){
                        endpoint = cr*LENGTH + cc;
                    }
                }

                getOpenDirections(cr,cc,vlines,hlines,dirs);

                int nr = -1, nc = -1; // next row next col
                


                // A limitation of this is that we are not detecting intersecting (T-type) chains(it would be way outside the
                // scope of this game, and happens very frequently. I honestly would not know how to do it,
                // as it would require DFS and even more analysis of the result to actually be able to traverse
                // it properly. Also, since this method is expected to be called every single turn, it should not
                // be an issue )

                    // up
                if (dirs[0] && cr > 0 && !(cr -1 == pr && cc == pc) && !chainArr[cr-1][cc] && owned[cr-1][cc] == '\0'){
                    nDegree = countOpenSides(vlines, hlines, cr-1, cc);
                    if (nDegree == 1 || nDegree ==2){
                        nr = cr-1;
                        nc = cc;
                    }

                    // down
                } else if (dirs[1] && !(cr+1 == pr && cc == pc) &&cr < HEIGHT-1 && !chainArr[cr+1][cc] && owned[cr+1][cc] == '\0'){
                    nDegree = countOpenSides(vlines, hlines, cr+1, cc);
                    if (nDegree == 1 || nDegree ==2){
                        nr = cr+1;
                        nc = cc;
                    }

                   // left 
                } else if (dirs[2] && !(cr == pr && cc-1 == pc)&& cc > 0 &&!chainArr[cr][cc-1] && owned[cr][cc-1] == '\0'){
                    nDegree = countOpenSides(vlines, hlines, cr, cc-1);
                    if (nDegree == 1 || nDegree ==2){
                        nr = cr;
                        nc = cc-1;
                    }

                    // right
                } else if (dirs[3] && !(cr == pr && cc+1 == pc) && cc < LENGTH-1 && !chainArr[cr][cc+1] && owned[cr][cc+1] == '\0'){
                    nDegree = countOpenSides(vlines, hlines, cr, cc+1);
                    if (nDegree == 1 || nDegree ==2){
                        nr = cr;
                        nc = cc+1;
                    }
                }

                if(nr == -1){ // nr cannot be -1 if any box was found, so having it -1 means none were found
                    break;
                }

                pr = cr;
                pc = cc;

                cr = nr;
                cc = nc;
            }

            //now store the info in the chains array
            chains[chainNum].length = chainLength;
            chains[chainNum].open = (endpointCount > 0);
            chains[chainNum].endpoint = endpoint;

            chainNum++;
        } 
    } 
}
