#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "seq_test.h"

void seqTest(int N, double** M, int numRuns){

    printf("-------------------------\n");
    printf("Sequential implementation\n");
    printf("-------------------------\n");

    //Check if matrix is symmetric
    if (checkSymSeq(M, N)) {
        printf("Matrix is symmetric: no need to transpose\n");
        return;
    }
    //Sequential transpose
    printf("Matrix is not symmetric\n");
    for (int i = 0; i < numRuns; i++){
        double** T = matTranspose(M, N);
        if (!isTransposed(M, T, N)) {
            fprintf(stderr, "%s", "Error: Sequential transpose failed\n");
            exit(1);
        }
    }
    return;
}