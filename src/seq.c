#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "time.h"

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

//Matrix randomization function
void matRandomize(double** M, int N){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            M[i][j] = (double) rand() / RAND_MAX;
        }
    }
}

//Simple checkSym function with the wall time measurement
int checkSymSeq(double** A, int N, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] != A[j][i]){
                clock_gettime(CLOCK_REALTIME, &end);
                double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
                *time = elapsed;
                return 0;
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return 1;
}

//Simple sequential transpose function
double** matTranspose(double** M, int N, double* time){

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
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            T[i][j] = M[j][i];
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
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