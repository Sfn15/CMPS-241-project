#include "helper.h"
#include "bot.h"
#include <malloc.h>
#include <stdio.h>

void nothing(){}

#define printf(...) nothing()

//TODO: FIX DOUBLE CROSS THINKING IT'S ENDGAME BECAUSE FILLED BLOCKS ARE NOT PART OF CHAIN

//function prototypes
int placeLine(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int box);
int placeLine2(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int r, int c);
int doubleCross(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int box1, int box2);
int countOpenSides(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int r, int c);
void getOpenDirections(int r, int c, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int dirs[4]);
void findChains(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH], struct chain chains[HEIGHT*LENGTH]);


int moveA(char p, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH]){
    int move = 0; //return 0 1 2 3 as 0123 (take into consideration leading zeros) // I am absolutely disgusted by this -stephane
    struct chain chains[HEIGHT*LENGTH];

    //initialize to 0
    for (int i = 0; i < HEIGHT * LENGTH; i++) {
        chains[i].length = 0;
        chains[i].open = 0;
        chains[i].endpoint = -1;
    }

    findChains(vlines, hlines, owned, chains);
    
    //TODO: (FIX) if early game OR last chain DO NOT execute double cross
    //count filled boxes
    int filled = 0;
    for(int i=0; i < HEIGHT; i++) {
        for(int j=0; j < LENGTH; j++) {
            if(owned[i][j] != '\0') filled++;
        }
    }

    printf("thinking...\n");
    
    //1. FIND LONGEST OPEN LONG CHAIN (length >= 3)
    printf("finding longest open long chain...\n");
    int max = 0;
    int total = 0;
    int chainCount = 0;
    for(int i=0; i < HEIGHT*LENGTH; i++) {
        total += chains[i].length;
        if(chains[i].length > 0) chainCount++;
        if(chains[i].open == 1 && chains[i].length > chains[max].length) {
            max = i;
        }
    }
    if(chains[max].open == 1 && chains[max].length >= 3) {
        printf("capture open chain of length %d\n", chains[max].length);
        return placeLine(vlines, hlines, chains[max].endpoint);
    }
    //2. DOUBLE CROSS
    else if(chains[max].open == 1 && chains[max].length == 2) {
        printf("%d + %d = %d", total, filled, total + filled);
        if(total + filled != HEIGHT*LENGTH || chainCount == 1) { //if not endgame or last chain
            printf("capture open chain of length 2\n");
            return placeLine(vlines, hlines, chains[max].endpoint);
        }
        else { //if endgame and not last chain
            printf("execute double cross\n");
            //TODO: (FIX) if early game OR last chain DO NOT execute double cross
            int o = 0;
            if(chains[max].blocks[0] == chains[max].endpoint) o++;
            move = doubleCross(vlines, hlines, chains[max].endpoint, chains[max].blocks[o]);
            if(move != -1) {return move;}
            else {printf("double cross cannot be executed, take that chain\n");}
            move = placeLine(vlines, hlines, chains[max].endpoint);
            if(move != -1) {return move;}
        }
    }
    //3. CLOSE SINGLE BOX
    else if(chains[max].open == 1 && chains[max].length == 1) {
        printf("close single box\n");
        return placeLine(vlines, hlines, chains[max].blocks[0]);
    }
    printf("no open chains found\n");
    //if there are no open chains, do the least bad move

    //FIND BOX WITH THE LEAST AMOUNT OF LINES
    //if 0 or 1, place line. this is early game stage.
    //if 2, look for shortest chain. this is late game stage.

    //FIND BOX WITH THE LEAST AMOUNT OF LINES
    //(most amount of openings)
    int min[3] = {0, 0, 5}; //{r, c, count}
    for(int i=0; i < HEIGHT; i++) {
        for(int j=0; j < LENGTH; j++) {
            if(owned[i][j] != '\0') continue;
            
            int n = 4 - countOpenSides(vlines, hlines, i, j);
            if(n < min[2]) {
                min[0] = i;
                min[1] = j;
                min[2] = n;
            }
        }
    }
    printf("box with least amount of lines is at r: %d, c: %d, and has %d lines\n", min[0], min[1], min[2]);
    //if 0 or 1, place line. this is early game stage.
    if(min[2] <= 1) {
        //check vlines
        int bestLine[4] = {0, 0, 5, 0}; //4th: 0: v , 1: h
        for(int i=0; i < HEIGHT; i++) {
            for(int j=0; j < LENGTH+1; j++) {
                if(vlines[i][j] != '\0') continue;
                //n = biggest number of lines in boxes adjacent to this line
                int n = 0;
                if(j < 0) n = 4 - countOpenSides(vlines, hlines, i, j);
                if(j > 0) {
                    int m = 4 - countOpenSides(vlines, hlines, i, j-1);
                    if(m > n) n = m;
                }
                //find smallest n
                if(n < bestLine[2]) {
                    bestLine[0] = i;
                    bestLine[1] = j;
                    bestLine[2] = n;
                }
            }
        }
        //check hlines
        for(int i=0; i < HEIGHT+1; i++) {
            for(int j=0; j < LENGTH; j++) {
                if(hlines[i][j] != '\0') continue;
                //n = biggest number of lines in boxes adjacent to this line
                int n = 0;
                if(i < 0) n = 4 - countOpenSides(vlines, hlines, i, j);
                if(i > 0) {
                    int m = 4 - countOpenSides(vlines, hlines, i-1, j);
                    if(m > n) n = m;
                }
                //find smallest n
                if(n < bestLine[2]) {
                    bestLine[0] = i;
                    bestLine[1] = j;
                    bestLine[2] = n;
                    bestLine[3] = 1;
                }
            }
        }
        //return
        if(bestLine[3] == 0) { //vline
            return bestLine[0]*1000 + bestLine[1]*100 + bestLine[0]*10 + 10 + bestLine[1];
        }
        else { //hline
            return bestLine[0]*1000 + bestLine[1]*100 + bestLine[0]*10 + bestLine[1] + 1;
        }
    }
    
    //if 2, look for shortest chain. this is late game stage.
    else {
        int shortest = 0;
        for(int i=0; i < HEIGHT * LENGTH; i++) {
            if(chains[i].length < chains[shortest].length || chains[shortest].length < 1) {
                shortest = i;
            }
        }
        printf("shortest: chain #%d with %d blocks\n", shortest, chains[shortest].length);
        if(chains[shortest].length != 0) {
            return placeLine(vlines, hlines, chains[shortest].blocks[0]);
        }
        else {
            return placeLine2(vlines, hlines, min[0], min[1]);
        }
    }
    //function ends here
}


//returns dots where a line can be placed on the side of the box
int placeLine(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH],int box){
    //box int to coords
    int r = box/LENGTH;
    int c = box%LENGTH;
    printf("box #%d is at r: %d, c: %d\n", box, r, c);

    if(hlines[r][c] =='\0'){ // up
        return 1000*r + 100*c + 10*r + c+1;
    } 
    if(hlines[r+1][c] == '\0'){ // down
        return 1000*(r+1) + 100*c + 10*(r+1) + c+1;
    }
    if(vlines[r][c] == '\0'){ // left
        return 1000*r + 100*c + 10*(r+1) + c;
    }
    if(vlines[r][c+1] == '\0'){ // right
        return 1000*r + 100*(c+1) + 10*(r+1) + c+1;
    }
    return -1;
}
//uses coords instead of box index
int placeLine2(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int r, int c){
    if(hlines[r][c] =='\0'){ // up
        return 1000*r + 100*c + 10*r + c+1;
    } 
    if(hlines[r+1][c] == '\0'){ // down
        return 1000*(r+1) + 100*c + 10*(r+1) + c+1;
    }
    if(vlines[r][c] == '\0'){ // left
        return 1000*r + 100*c + 10*(r+1) + c;
    }
    if(vlines[r][c+1] == '\0'){ // right
        return 1000*r + 100*(c+1) + 10*(r+1) + c+1;
    }
    return -1;
}

int doubleCross(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], int box1, int box2){
    //box int to coords
    //int r1 = box1/LENGTH;
    //int c1 = box1%LENGTH;
    int r2 = box2/LENGTH;
    int c2 = box2%LENGTH;
    //box1 is the endpoint

    //printf("endpoint: box #%d (%d, %d) | box #%d (%d, %d)", box1, r1, c1, box2, r2, c2);

    //trying to find an empty line on box2 that isn't adjacent to box1
    if(hlines[r2][c2] =='\0' && box1 != box2-LENGTH){ // up
        return 1000*r2 + 100*c2 + 10*r2 + c2+1;
    } 
    if(hlines[r2+1][c2] == '\0' && box1 != box2+LENGTH){ // down
        return 1000*(r2+1) + 100*c2 + 10*(r2+1) + c2+1;
    }
    if(vlines[r2][c2] == '\0' && box1 != box2-1){ // left
        return 1000*r2 + 100*c2 + 10*(r2+1) + c2;
    }
    if(vlines[r2][c2+1] == '\0' && box1 != box2+1){ // right
        return 1000*r2 + 100*(c2+1) + 10*(r2+1) + c2+1;
    }
    return -1;
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
