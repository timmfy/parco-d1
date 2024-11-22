#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "imp_par.h"
#include "imp_par_test.h"

void impParTest(int N, int blockSize, double* M, int numRuns){
    printf("-------------------------\n");
    printf("Implicit parallel implementation\n");
    printf("-------------------------\n");
    double time;
    double avgTime = 0;
    for (int i = 0; i < numRuns; i++){
        // Check if matrix is symmetric
        if (checkSymImpTime(M, N, blockSize, &time)){
            printf("Matrix is symmetric: no need to transpose\n");
            printf("Time to check symmetry: %.9f\n", time);
            return;
        }
        double* T = matTransposeImp(M, N, blockSize, &time);
        printf("Time to transpose: %.9f\n", time);
        avgTime += time;
        if (!isTransposed(M, T, N)) {
            fprintf(stderr, "%s", "Error: Parallel transpose failed\n");
            exit(1);
        }
        matRandomize(M, N);
        free(T);
    }
    avgTime /= numRuns;
    printf("Average time of %d runs: %.9f\n", numRuns, avgTime);
}