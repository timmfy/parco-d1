#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "seq.h"
#include "seq_test.h"
#include "imp_par_test.h"
#include "omp_par_test.h"

int main(int argc, char** argv){
    int blockSize = 4; // Default block size
    int numRuns = 1; // Default number of runs
    int genSym = 0; // Generate symmetric matrix (0 by default)
    int test_seq = 0; // Run sequential test by default
    int test_imp = 0; // Run implicit parallel test by default
    int test_exp = 0; // Run explicit parallel test by default
    int ompThreads = 4; // Default number of threads

    // Set the execution parameters
    for (int i = 1; i < argc; i++){
        //printf("Argument: %s\n", argv[i]);
        // Set the block size
        if (strcmp(argv[i], "--block-size") == 0){
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
                    case 'o':
                        test_exp = 1;
                        break;
                    default:
                        fprintf(stderr, "Error: Invalid test option '%c'\n", argv[i][j]);
                        exit(1);
                }
            }
        }
        else if (strcmp(argv[i], "--threads") == 0){
            if (i + 1 < argc){
                ompThreads = atoi(argv[i + 1]);
                if (ompThreads <= 0){
                    fprintf(stderr, "%s", "Error: Number of threads must be greater than 0\n");
                    exit(1);
                }
                i++;
            }
            else{
                fprintf(stderr, "%s", "Error: --threads flag requires an argument\n");
                exit(1);
            }
        }
        else {
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

    // Run the tests
    double seqTime = 0.0, impTime = 0.0, ompTime = 0.0;
    if (test_seq) {
        seqTime = seqTest(numRuns, genSym);
        printf("Sequential time: %.9f\n", seqTime);
    }
    if (test_imp) {
        impTime = impParTest(blockSize, numRuns, genSym);
        printf("Implicit parallel time: %.9f\n", impTime);
    }
    if (test_exp) {
        ompTime = ompParTest(blockSize, ompThreads, numRuns, genSym);
        printf("OMP parallel time: %.9f\n", ompTime);
    }
    if ((test_seq) && (test_imp)){
        printf("-------------------------\n");
        printf("Speedup implicit parallel: %.9f\n", seqTime / impTime);
    }
    if ((test_seq) && (test_exp)){
        printf("Speedup explicit parallel: %.9f\n", seqTime / ompTime);
    }
    printf("-------------------------\n");
    return 0;
}