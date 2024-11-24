#include "stdio.h"
#include "stdlib.h"
#include "imp_par.h"
#include "time.h"

// checkSym function with implicit parallelism and optimized for cache
int checkSymImp(double* A, int N, int blockSize){
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int ii = i; ii < i + blockSize && ii < N; ii++){
                for(int jj = j; jj < j + blockSize && jj < N; jj++){
                    if(A[ii * N + jj] != A[jj * N + ii]){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// time measurement for checkSymImp
int checkSymImpTime(double* A, int N, int blockSize, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    int result = checkSymImp(A, N, blockSize);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return result;
}

double* matTransposeImp(double* A, int N, int blockSize, double* time){
    double* B = (double*)malloc(N * N * sizeof(double));
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int ii = i; ii < i + blockSize && ii < N; ii++){
                for(int jj = j; jj < j + blockSize && jj < N; jj++){
                    B[ii * N + jj] = A[jj * N + ii];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return B;
}

double matTransposeImpDiag(double* A, int N, double* time){}