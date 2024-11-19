#include "stdio.h"
#include "stdlib.h"
#include "matrix_utils.h"
#include "omp.h"

//Random matrix generation function
double** matGenerate(int N){
    double** A = (double**) malloc(N * sizeof(double*));
    if(A == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    for(int i = 0; i < N; i++){
        A[i] = (double*) malloc(N * sizeof(double));
        if(A[i] == NULL){
            printf("Memory allocation failed\n");
            exit(1);
        }
        for(int j = 0; j < N; j++){
            A[i][j] = (double) rand() / RAND_MAX;
        }
    }
    return A;
}

//Simple checkSym function
int checkSym(double** A, int N){
    double start, end;
    start = omp_get_wtime();
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] != A[j][i]){
                end = omp_get_wtime();
                printf("Time taken for sequential checkSym: %f\n", end - start);
                return 0;
            }
        }
    }
    end = omp_get_wtime();
    printf("Time taken for checkSym: %f\n", end - start);
    return 1;
}