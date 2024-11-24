#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "seq_test.h"

void seqTest(int N, double** M, int numRuns){

    printf("-------------------------\n");
    printf("Sequential implementation\n");
    printf("-------------------------\n");
    double time;
    //Check if matrix is symmetric
    if (checkSymSeq(M, N, &time)){
        printf("Matrix is symmetric: no need to transpose\n");
        printf("Time to check symmetry: %.9f\n", time);
        return;
    }
    //Sequential transpose
    printf("Matrix is not symmetric\n");
    double avgTime = 0;
    for (int i = 0; i < numRuns; i++){
        double time;
        double** T = matTranspose(M, N, &time);
        printf("Time to transpose: %.9f\n", time);
        avgTime += time;
        if (!isTransposed(M, T, N)) {
            fprintf(stderr, "%s", "Error: Sequential transpose failed\n");
            exit(1);
        }
    }
    avgTime /= numRuns;
    printf("Average time of %d runs: %.9f\n", numRuns, avgTime);
    return;
}