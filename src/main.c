#include "stdio.h"
#include "stdlib.h"
#include "seq_test.h"
#include "string.h"
#include "imp_par_test.h"
#include "seq.h"
#include "math.h"

int main(int argc, char** argv){
    int N = (int)(pow(2, 10)); // Default matrix size
    int blockSize = 4; // Default block size
    int numRuns = 1; // Default number of runs
    int genSym = 0; // Generate symmetric matrix (0 by default)
    int test_seq = 0; // Run sequential test by default
    int test_imp = 0; // Run implicit parallel test by default
    int test_exp = 0; // Run explicit parallel test by default

    // Set the execution parameters
    for (int i = 1; i < argc; i++){
        printf("Argument: %s\n", argv[i]);
        // Set the matrix size
        if (strcmp(argv[i], "--size") == 0){
            if (i + 1 < argc){
                N = atoi(argv[i + 1]);
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --size flag requires an argument\n");
                exit(1);
            }
        }
        // Set the block size
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
        // Set the number of runs
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
        // Generate symmetric matrix
        else if (strcmp(argv[i], "--symm") == 0){
            if (i + 1 < argc){
                genSym = atoi(argv[i + 1]);
                if (genSym != 0 && genSym != 1){
                    fprintf(stderr, "%s", "Error: Invalid argument value\n");
                    exit(1);
                }
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --symm flag requires an argument\n");
                exit(1);
            }
        }
        // Select tests to run
        else if ((argv[i][0] == '-') && (argv[i][1] != '-')) {
            for (int j = 1; j < (int)strlen(argv[i]); j++) {
                switch (argv[i][j]) {
                    case 's':
                        test_seq = 1;
                        break;
                    case 'i':
                        test_imp = 1;
                        break;
                    case 'e':
                        test_exp = 1;
                        break;
                    default:
                        fprintf(stderr, "Error: Invalid test option '%c'\n", argv[i][j]);
                        exit(1);
                }
            }
        } else {
            fprintf(stderr, "Error: Unknown argument '%s'\n", argv[i]);
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

    // Generate random matrix
    double* M;
    if (genSym == 1){
        M = matGenerateSym(N);
    }
    else{
        M = matGenerate(N);
    }

    // Run the tests
    if (test_seq) {
        seqTest(N, M, numRuns);
    }
    if (test_imp) {
        impParTest(N, blockSize, M, numRuns);
    }
    if (test_exp) {
        fprintf(stderr, "%s", "Error: Explicit parallel implementation not available\n");
        exit(1);
    }

    free(M);
    return 0;
}