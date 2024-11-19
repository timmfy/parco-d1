#include "stdio.h"
#include "stdlib.h"
#include "matrix_utils.h"
#include "omp.h"

//Random matrix generation function
double** matGenerate(int N){
    printf("Generating random matrix\n");
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

//Random symmetric matrix generation function
double** matGenerateSym(int N){
    printf("Generating random symmetric matrix\n");
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
        for(int j = 0; j < i; j++){
            A[i][j] = (double) rand() / RAND_MAX;
            A[j][i] = A[i][j];
        }
        A[i][i] = (double) rand() / RAND_MAX;
    }
    return A;
}

//Simple checkSym function
int checkSymSeq(double** A, int N){
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
    printf("Time taken for sequential checkSym: %f\n", end - start);
    return 1;
}

//checkSym function with implicit parallelism and optimized for cache
int checkSymImp(double** A, int N, int blockSize){
    double start, end;
    start = omp_get_wtime();
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int ii = i; ii < i + blockSize; ii++){
                for(int jj = j; jj < j + blockSize; jj++){
                    if(A[ii][jj] != A[jj][ii]){
                        end = omp_get_wtime();
                        printf("Time taken for implicit parallel checkSym: %f\n", end - start);
                        return 0;
                    }
                }
            }
        }
    }
    end = omp_get_wtime();
    printf("Time taken for implicit parallel checkSym: %f\n", end - start);
    return 1;
}

//checkSym function with explicit parallelism using OpenMP
int checkSymExp(double** A, int N, int blockSize){
    double start, end;
    int result = 1;
    start = omp_get_wtime();
    #pragma omp parallel for collapse(2) schedule(static, blockSize) shared(result)
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if (result == 0) {
                #pragma omp cancel for 
            }
            if(A[i][j] != A[j][i]){
                end = omp_get_wtime();
                #pragma omp critical
                {
                    result = 0;
                }
                #pragma omp cancel for
            }
        }
    }
    end = omp_get_wtime();
    printf("Time taken for explicit parallel checkSym: %f\n", end - start);
    return result;
}