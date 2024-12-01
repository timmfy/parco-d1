#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "time.h"
#include "omp_par.h"

//checkSym using OpenMP
#include <omp.h>

int checkSymOMP(double* M, int numThreads){

    int symmetric = 1;
    #pragma omp parallel for shared(symmetric) num_threads(numThreads) schedule(dynamic)
    for (int i = 0; i < N; i++){
        if (!symmetric) continue;
        for (int j = i + 1; j < N; j++){
            if (M[i*N + j] != M[j*N + i]){
                #pragma omp critical
                symmetric = 0;
                break;
            }
        }
    }
    return symmetric;
}
int checkSymOMPTime(double* M, int numThreads, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    int symmetric = checkSymOMP(M, numThreads);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return symmetric;
}
//Matrix transpose using OpenMP with cache blocking
double* matTransposeOMP(double* M, int blockSize, int numThreads, double* time){
    struct timespec start, end;
    double* T = (double*)malloc(N*N*sizeof(double));
    clock_gettime(CLOCK_REALTIME, &start);
    #pragma omp parallel for schedule(dynamic) num_threads(numThreads)
    for (int i = 0; i < N; i += blockSize) {
        for (int j = 0; j < N; j += blockSize) {
            for (int ii = i; ii < i + blockSize; ii++) {
                for (int jj = j; jj < j + blockSize; jj++) {
                    //Non-sequential access to M
                    T[ii*N + jj] = M[jj*N + ii];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return T;
}

//Cache oblivious transpose routine with parallel sections
void transpose(double* M, double* T, int i, int j, int dim, int blockSize){
    if (dim <= blockSize){
        for (int ii = i; ii < i + dim; ii++){
            for (int jj = j; jj < j + dim; jj++){
                T[ii*N + jj] = M[jj*N + ii];
            }
        }
        return;
    }
    #pragma omp parallel sections num_threads(16)
    {
        #pragma omp section
        transpose(M, T, i, j, dim/2, blockSize);
        #pragma omp section
        transpose(M, T, i + dim/2, j + dim/2, dim/2, blockSize);
        #pragma omp section
        transpose(M, T, i, j + dim/2, dim/2, blockSize);
        #pragma omp section
        transpose(M, T, i + dim/2, j, dim/2, blockSize);
    }
}
//Matrix transpose using OpenMP with cache oblivious algorithm
double* matTransposeOMPCacheOblivious(double* M, int blockSize, double* time){
    struct timespec start, end;
    double* T = (double*)malloc(N*N*sizeof(double));
    if (T == NULL){
        printf("Error: Memory allocation failed\n");
        fprintf(stderr, "%s", "Error: Memory allocation failed\n");
        exit(1);
    }
    clock_gettime(CLOCK_REALTIME, &start);
    int dim = N;
    transpose(M, T, 0, 0, dim, blockSize);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return T;
}