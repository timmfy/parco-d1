#include "stdio.h"
#include "stdlib.h"
#include "omp.h"
#include "time.h"
#include "omp_par.h"

//checkSym using OpenMP
#include <omp.h>

int checkSymOMP(double* M, int blockSize, int numThreads){

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
int checkSymOMPTime(double* M, int blockSize, int numThreads, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    int symmetric = checkSymOMP(M, blockSize, numThreads);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return symmetric;
}
//Matrix transpose using OpenMP
double* matTransposeOMP(double* M, int blockSize, int numThreads, double* time){
    struct timespec start, end;
    double* T = (double*)malloc(N*N*sizeof(double));
    clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel for schedule(dynamic) num_threads(numThreads)
    for (int i = 0; i < N; i += blockSize) {
        for (int j = 0; j < N; j += blockSize) {
            for (int ii = i; ii < i + blockSize; ii++) {
                #pragma omp simd aligned(T, M:64)
                for (int jj = j; jj < j + blockSize; jj++) {
                    T[jj*N + ii] = M[ii*N + jj];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return T;
}