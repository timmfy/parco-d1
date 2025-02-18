#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "imp_par.h"
#include "imp_par_test.h"

double impParTest(int blockSize, int numRuns, int symmetric){
    printf("-------------------------\n");
    printf("Implicit parallel implementation\n");
    printf("-------------------------\n");
    double time;
    double avgTime = 0;
    double *M;
    if (symmetric)
        M = matGenerateSym();
    else
        M = matGenerate();
    for (int i = 0; i < numRuns; i++){
        // Check if matrix is symmetric
        if (checkSymImpTime(M, blockSize, &time)){
            avgTime += time;
            M = matGenerateSym();
            continue;
        }
        double* T = matTransposeImpCacheObliviousTime(M, blockSize, &time);
        avgTime += time;
        if (!isTransposed(M, T)) {
            fprintf(stderr, "%s", "Error: Parallel transpose failed\n");
            exit(1);
        }
        matRandomize(M);
        free(T);
    }
    avgTime /= numRuns;
    printf("Average time of %d runs: %.9f\n", numRuns, avgTime);
    free(M);
    return avgTime;
}