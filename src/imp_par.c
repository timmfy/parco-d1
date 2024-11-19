#include "stdio.h"
#include "stdlib.h"
#include "matrix_utils.h"
#include "transposition.h"
#include "imp_par.h"

void parTest(int N, int blockSize){

    printf("-------------------------\n");
    printf("Parallel implementation\n");
    printf("-------------------------\n");
    double** A = matGenerateSym(N);
    if (checkSymImp(A, N, blockSize)) {
        for (int i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
    }
    else{
        fprintf(stderr, "%s", "Error: Random symmetric matrix is not symmetric\n");
        exit(1);
    }
    double** B = matGenerate(N);
    if (checkSymImp(B, N, blockSize)) {
        for (int i = 0; i < N; i++) {
            free(B[i]);
        }
        free(B);
        fprintf(stderr, "%s", "Error: Random matrix is symmetric\n");
        exit(1);
    }
    double** C = matTranspose(B, N);
    if (!isTransposed(B, C, N)) {
        fprintf(stderr, "%s", "Error: Parallel transpose failed\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        free(B[i]);
        free(C[i]);
    }
    free(B);
    free(C);

}