#include "stdio.h"
#include "stdlib.h"
#include "matrix_utils.h"
#include "transposition.h"
#include "sequential.h"

void seqImplTest(int N){
    double** A = matGenerate(N);
    if (checkSym(A, N)) {
        for (int i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
    }

    //Sequential transpose
    double** B = matTranspose(A, N);
    if (!isTransposed(A, B, N)) {
        fprintf(stderr, "%s", "Error: Sequential transpose failed\n");
        exit(1);
    }
    //Cleanup
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
}