#include "stdio.h"
#include "stdlib.h"
#include "sequential.h"
#include "math.h"
int main(int argc, char** argv){
    
    if (argc != 2){
        fprintf(stderr, "%s", "Error: Invalid number of arguments\n");
        exit(1);
    }
    int N = atoi(argv[1]);
    if (N <= 0){
        fprintf(stderr, "%s", "Error: Invalid matrix size\n");
        exit(1);
    }

    seqTest((int)pow(2, N));
    parTest((int)pow(2, N), 8);
    return 0;
}   