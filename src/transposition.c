#include "transposition.h"
#include "matrix_utils.h"
#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
//Simple sequential transpose function
double** matTranspose(double** M, int N){

    //Allocate memory for the transposed matrix
    double** T = (double**) malloc(N * sizeof(double*));
    if (T == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        T[i] = (double*) malloc(N * sizeof(double));
        if (T == NULL){
            printf("Memory allocation failed\n");
            exit(1);
        }   
    }
    double start, end;
    start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            T[i][j] = M[j][i];
        }
    }
    end = omp_get_wtime();
    printf("Time taken for sequential matTranspose: %f\n", end - start);
    return T;
}
//Function to check if the matrix is transposed
int isTransposed(double** A, double** B, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] != B[j][i]){
                return 0;
            }
        }
    }
    return 1;
}

