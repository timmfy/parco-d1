#include "stdio.h"
#include "stdlib.h"
#include "seq_test.h"
#include "string.h"
#include "imp_par_test.h"
#include "math.h"
int main(int argc, char** argv){
    
    int N = (int)(pow(2, 10)); //Default matrix size
    int blockSize = 4; //Default block size
    int numRuns = 1; //Default number of runs
    int test = 0; //Default test to run (0 = all, 1 = sequential, 2 = implicit parallel, 3 = explicit parallel)
    int genSym = 0; //Generate symmetric matrix (0 by default)
    //Set the execution parameters
    for (int i = 1; i < argc; i++){
        //Set the matrix size
        if (strcmp(argv[i], "--size") == 0){
            if (i + 1 < argc){
                N = (int)(pow(2, atoi(argv[i + 1])));
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --size flag requires an argument\n");
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
                fprintf(stderr, "%s", "Error: --block-size flag requires an argument\n");
                exit(1);
            }
        }
        //Select tests to run
        else if (strcmp(argv[i], "--test") == 0){
            if (i + 1 < argc){
                if (strcmp(argv[i + 1], "all") == 0){
                    test = 0;
                }
                else if (strcmp(argv[i + 1], "seq") == 0){
                    test = 1;
                }
                else if (strcmp(argv[i + 1], "imp") == 0){
                    test = 2;
                }
                else if (strcmp(argv[i + 1], "exp") == 0){
                    test = 3;
                }
                else{
                    fprintf(stderr, "%s", "Error: Invalid test\n");
                    exit(1);
                }
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --test flag requires an argument\n");
                exit(1);
            }
        }
        //Set the number of runs
        else if (strcmp(argv[i], "--runs") == 0){
            if (i + 1 < argc){
                numRuns = atoi(argv[i + 1]);
                if (numRuns <= 0){
                    fprintf(stderr, "%s", "Error: Number of runs must be greater than 0\n");
                    exit(1);
                }
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --runs flag requires an argument\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--symm") == 0){
            if (i + 1 < argc){
                genSym = atoi(argv[i + 1]);
                if (genSym != 0 && genSym != 1){
                    fprintf(stderr, "%s", "Error: Invalid argument value\n");
                    exit(1);
                }
                i++;
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
    if (genSym == 1){
        for (int i = 0; i < N; i++){
            M[i] = (double*)malloc(N * sizeof(double));
            for (int j = 0; j < N; j++){
                M[i][j] = (double) i * j; 
            }
        }
    }
    else{
        for (int i = 0; i < N; i++){
            M[i] = (double*)malloc(N * sizeof(double));
            for (int j = 0; j < N; j++){
                M[i][j] = (double)rand() / RAND_MAX;
            }
        }
    }
    
    //Run the tests
    if (test == 0){
        seqTest(N, M, numRuns);
        impParTest(N, blockSize, M, numRuns);
    }
    else if (test == 1){
        seqTest(N, M, numRuns);
    }
    else if (test == 2){
        impParTest(N, blockSize, M, numRuns);
    }
    else if (test == 3){
        fprintf(stderr, "%s", "Error: Explicit parallel implementation not available\n");
        exit(1);
    }
    for (int i = 0; i < N; i++){
        free(M[i]);
    }
    free(M);
    return 0;
}