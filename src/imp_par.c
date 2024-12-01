#include "stdio.h"
#include "stdlib.h"
#include "imp_par.h"
#include "time.h"

// checkSym function with cache blocking

int checkSymImp(double* A, int blockSize){
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
// checkSym function with loop unroll
int checkSymImpUnroll(double* A, int blockSize){
    for (int i = 0; i < N; i++){
        for (int j = i + 1; j < N; j += blockSize){
            for (int k = 0; k < blockSize; k++){
                if (A[i * N + j + k] != A[(j + k) * N + i]){
                    return 0;
                }
            }
        }
    }
}

// time measurement for checkSymImp
int checkSymImpTime(double* A, int blockSize, double* time){
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    int result = checkSymImp(A, blockSize);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return result;
}
//Implementation of matrix transpose with cache blocking and vectorization
double* matTransposeImp(double* A, int blockSize, double* time){
    double* B = (double*)malloc(N * N * sizeof(double));
    if (B == NULL){
        fprintf(stderr, "%s", "Error: Memory allocation failed\n");
        exit(1);
    }
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int ii = i; ii < i + blockSize; ii++){
                for(int jj = j; jj < j + blockSize; jj++){
                    //Contiguous access to A
                    B[jj * N + ii] = A[ii * N + jj];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return B;
}
//Implementation of the cache oblivious matrix transpose with vectorization
double* matTransposeImpCacheOblivious(double* A, double* B, int i, int j, int dim, int blockSize){
    if(dim <= blockSize){
        for(int ii = i; ii < i + dim; ii++){
            for(int jj = j; jj < j + dim; jj++){
                B[jj * N + ii] = A[ii * N + jj];
            }
        }
        return B;
    }
    int half = dim / 2;
    matTransposeImpCacheOblivious(A, B, i, j, half, blockSize);
    matTransposeImpCacheOblivious(A, B, i, j + half, half, blockSize);
    matTransposeImpCacheOblivious(A, B, i + half, j, half, blockSize);
    matTransposeImpCacheOblivious(A, B, i + half, j + half, half, blockSize);
    return B;
}
double* matTransposeImpCacheObliviousTime(double* A, int blockSize, double* time){
    double* B = (double*)malloc(N * N * sizeof(double));
    if (B == NULL){
        fprintf(stderr, "%s", "Error: Memory allocation failed\n");
        exit(1);
    }
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    matTransposeImpCacheOblivious(A, B, 0, 0, N, blockSize);
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    *time = elapsed;
    return B;
}