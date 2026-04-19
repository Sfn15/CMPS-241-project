#include"helper.h"

struct chain{
    int open;
    int length;
    int blocks[HEIGHT*LENGTH];

    int endpoint;
};

int moveA(char p, char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH],int coord[3]);

void findChains(char vlines[HEIGHT][LENGTH+1], char hlines[HEIGHT+1][LENGTH], char owned[HEIGHT][LENGTH], struct chain chains[HEIGHT*LENGTH]);









/*
Chain checking:

find chains somehow

to store chains, create a custom struct, which will store 20 arrays, corresponding to each one of the boxes

each entry contains the following information about the chain:

length
chain number (even if the chain is open (open for the taking)), odd otherwise - and 0 if the box does not belong to any chain


information about a chain will be stored at the indices that correspond to its member boxes (i.e. if box N belongs to chain C, then at index N in the array),
we will find the chain number


the strategy will then be implemented using the information gathered here.

If a chain is not found, we resort to basic moves






To find chains, begin a "dfs" by starting at a box, checking if it is surrounded by at least 2 lines. (MAKE SURE TO CHECK EDGE CASES)

If it is, start looking for its neighbors (in the direction of the chain's opening), and mark each visited box as visited in a copy of the boxes array

-1 if it was not added to the chain, 1 if it was, and 0 if not visited yet. 

once you have checked all possible lines, collect the information and insert it in the chain storage struct. 
*/




