//Test the OMP parallel implementation of the matrix transposition
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

double* matTranspose(double* mat, double* time) {
    double* transposed = (double*)malloc(N * N * sizeof(double));
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transposed[i * N + j] = mat[j * N + i];
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    *time = elapsed;
    return transposed;
}

double* matTransposeCacheBlocking(double* mat, double* time) {
    double* transposed = (double*)malloc(N * N * sizeof(double));
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_REALTIME, &start);
    #pragma omp parallel for schedule(guided,16) num_threads(numThreads)
    for (int i = 0; i < N; i += blockSize) {
	//printf("Thread %d is processing block (%d, %d)\n", thread_num, i, j);
	 //int thread_num = omp_get_thread_num();
	//printf("Thread %d is processing row %d\n", thread_num, i);
        for (int j = 0; j < N; j += blockSize) {
	    int thread_num = omp_get_thread_num();
            printf("Thread %d is processing block (%d, %d) to (%d, %d)\n", thread_num, i, j, i + blockSize - 1, j + blockSize - 1);
            for (int ii = i; ii < i + blockSize; ii++) {
                //#pragma omp simd aligned(transposed, mat:64)
                for (int jj = j; jj < j + blockSize; jj++) {
		        //int thread_num = omp_get_thread_num();
		        //printf("Thread %d is processing element (%d, %d)\n", thread_num, ii, jj);
                    transposed[ii * N + jj] = mat[jj * N + ii];
                
                }
            }
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    *time = elapsed;
    return transposed;
}

int main() {
    double* mat = (double*)malloc(N * N * sizeof(double));
    double avgTime = 0.0;
    double time = 0.0;
    for (int i = 0; i < numRuns; i++){
        // Randomize matrix
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                mat[i * N + j] = (double)rand() / RAND_MAX;
            }
        }
        double* transposed = matTransposeCacheBlocking(mat, &time);
        avgTime += time;
    }
    avgTime /= numRuns;
    printf("Average time of %d runs: %.9f\n", numRuns, avgTime);
    free(mat);
    return 0;
}
