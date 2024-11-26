//Test the optimized version of the matrix transpose for the vectorization
//Using cache blocking and loop unrolling
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#define N 1024

void transpose_opt(double* A, int blockSize){
    double* B = (double*)malloc(N*N*sizeof(double));
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i+=blockSize){
        for(int j = 0; j < N; j+=blockSize){
            for(int ii = i; ii < i+blockSize; ii++){
                for(int jj = j; jj < j+blockSize; jj++){
                    B[jj * N + ii] = A[ii * N + jj];
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Elapsed time: %f\n", elapsed);
}
double* matTransposeImp(double* A, int blockSize, double* time){
    double* B = (double*)malloc(N * N * sizeof(double));
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int ii = i; ii < i + blockSize; ii++){
                for(int jj = j; jj < j + blockSize; jj++){
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
int main(int argc, char** argv){
    int BLOCK_SIZE = atoi(argv[1]);
    double* A = (double*)malloc(N*N*sizeof(double));
    double* B = (double*)malloc(N*N*sizeof(double));
    printf("Block size: %d\n", BLOCK_SIZE);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            A[i*N+j] = i+j;
        }
    }
    double time;
    double* B1 = matTransposeImp(A, BLOCK_SIZE, &time);
    free(B1);
    free(A);
    free(B);
    return 0;

}