#include "stdio.h"
#include "stdlib.h"
#include "matrix_utils.h"
#include "transposition.h"
#include "sequential.h"

void seqTest(int N){

    printf("-------------------------\n");
    printf("Sequential implementation\n");
    printf("-------------------------\n");
    //Generate random symmetric matrix
    double** SYM = matGenerateSym(N);
    if (checkSymSeq(SYM, N)) {
        for (int i = 0; i < N; i++) {
            free(SYM[i]);
        }
        free(SYM);
    }
    else{
        fprintf(stderr, "%s", "Error: Random symmetric matrix is not symmetric\n");
        exit(1);
    }
    //Generate random matrix
    double** A = matGenerate(N);
    if (checkSymSeq(A, N)) {
        for (int i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
        fprintf(stderr, "%s", "Error: Random matrix is symmetric\n");
        exit(1);
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