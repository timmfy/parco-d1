#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "seq_test.h"

double seqTest(double* M, int numRuns){
    printf("-------------------------\n");
    printf("Sequential implementation\n");
    printf("-------------------------\n");
    double time;
    double avgTime = 0;
    for (int i = 0; i < numRuns; i++){
        // Check if matrix is symmetric
        if (checkSymSeq(M, &time)){
            printf("Matrix is symmetric: no need to transpose\n");
            printf("Time to check symmetry: %.9f\n", time);
            return time;
        }
        // Sequential transpose
        double* T = matTranspose(M, &time);
        //printf("Time to transpose: %.9f\n", time);
        avgTime += time;
        // if (!isTransposed(M, T)) {
        //     fprintf(stderr, "%s", "Error: Sequential transpose failed\n");
        //     exit(1);
        // }
        matRandomize(M);
        free(T);
    }
    avgTime /= numRuns;
    printf("Average time of %d runs: %.9f\n", numRuns, avgTime);
    return avgTime;
}