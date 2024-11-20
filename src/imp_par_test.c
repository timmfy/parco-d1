#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "imp_par.h"
#include "imp_par_test.h"

void parTest(int N, int blockSize, double** M) {

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

    //Transpose matrix and verify if it is correct
    double** T = matTranspose(M, N);
    if (!isTransposed(M, T, N)) {
        fprintf(stderr, "%s", "Error: Parallel transpose failed\n");
        exit(1);
    }
    return;

}