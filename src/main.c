#include "stdio.h"
#include "stdlib.h"
#include "seq_test.h"
#include "string.h"
#include "imp_par_test.h"
#include "math.h"
int main(int argc, char** argv){
    
    int N = (int)(pow(2, 10)); //Default matrix size
    int blockSize = 4; //Default block size
    //Set the execution parameters
    for (int i = 1; i < argc; i++){
        //Set the matrix size
        if (strcmp(argv[i], "--size") == 0){
            if (i + 1 < argc){
                N = (int)(pow(2, atoi(argv[i + 1])));
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: -s flag requires an argument\n");
                exit(1);
            }
        }
        //Set the block size
        else if (strcmp(argv[i], "--block-size") == 0){
            if (i + 1 < argc){
                blockSize = atoi(argv[i + 1]);
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: -b flag requires an argument\n");
                exit(1);
            }
        }
        //Add later
        else{
            fprintf(stderr, "%s", "Error: Invalid flag\n");
            exit(1);
        }
    }
    printf("Matrix: %dx%d\n", N, N);
    if ((N <= 1) || (N > 4096)){
        fprintf(stderr, "%s", "Error: Invalid matrix size\n");
        exit(1);
    }
    printf("Block size: %d\n", blockSize);
    if ((blockSize <= 1) || (blockSize > 4096)){
        fprintf(stderr, "%s", "Error: Invalid block size\n");
        exit(1);
    }

    //Generate random matrix
    double** M = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++){
        M[i] = (double*)malloc(N * sizeof(double));
        for (int j = 0; j < N; j++){
            M[i][j] = (double)rand() / RAND_MAX;
        }
    }
    seqTest(N, M);
    parTest(N, blockSize, M);
    for (int i = 0; i < N; i++){
        free(M[i]);
    }
    free(M);
    return 0;
}   