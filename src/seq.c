#include "stdio.h"
#include "stdlib.h"
#include "seq.h"
#include "time.h"

// Random matrix generation function
double* matGenerate(){
    printf("Generating random matrix\n");
    double* A = (double*) malloc(N * N * sizeof(double));
    if(A == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    for(int i = 0; i < N * N; i++){
        A[i] = (double) rand() / RAND_MAX;
    }
    return A;
}

// Random symmetric matrix generation function
double* matGenerateSym(){
    printf("Generating random symmetric matrix\n");
    double* A = (double*) malloc(N * N * sizeof(double));
    if(A == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j <= i; j++){
            A[i * N + j] = (double) rand() / RAND_MAX;
            A[j * N + i] = A[i * N + j];
        }
    }
    return A;
}

// Matrix randomization function
void matRandomize(double* M){
    for (int i = 0; i < N * N; i++){
        M[i] = (double) rand() / RAND_MAX;
    }
}

// Simple checkSym function with the wall time measurement
int checkSymSeq(double* A, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i++){
        for(int j = i + 1; j < N; j++){
            if(A[i * N + j] != A[j * N + i]){
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

// Simple sequential transpose function
double* matTranspose(double* M, double* time){
    double* T = (double*) malloc(N * N * sizeof(double));
    if (T == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            T[i * N + j] = M[j * N + i];
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return T;
}

// Function to check if the matrix is transposed
int isTransposed(double* A, double* B){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i * N + j] != B[j * N + i]){
                return 0;
            }
        }
    }
    return 1;
}