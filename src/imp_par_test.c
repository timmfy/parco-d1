#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "imp_par.h"
#include "imp_par_test.h"

void impParTest(int N, int blockSize, double** M, int numRuns){

    printf("-------------------------\n");
    printf("Implicit parallel implementation\n");
    printf("-------------------------\n");

    //Check if matrix is symmetric
    if (checkSymImpTime(M, N, blockSize)) {
        printf("Matrix is symmetric: no need to transpose\n");
        return;
    }
    else{
        printf("Matrix is not symmetric\n");
    }

    for (int i = 0; i < numRuns; i++){
        //Transpose matrix and verify if it is correct
        double** T = matTransposeImp(M, N, blockSize);
        if (!isTransposed(M, T, N)) {
            fprintf(stderr, "%s", "Error: Parallel transpose failed\n");
            exit(1);
        }
    }
    return;

}