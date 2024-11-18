#include "stdio.h"
#include "stdlib.h"
#include "time.h"

// Matrix size
int N;
//Random matrix generation function
double** matGeneratev1(){
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
//checkSym function
int checkSym(double** A){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] != A[j][i]){
                return 0;
            }
        }
    }
    return 1;
}
// Matrix transpose function
// Transposes matrix A and stores the result in matrix B
void matTranspose(double** A, double** B){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = A[j][i];
        }
    }
}
//Check if the matrix is transposed
int isTransposed(double** A, double** B){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] != B[j][i]){
                return 0;
            }
        }
    }
    return 1;
}
int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr, "%s", "Error: Invalid number of arguments\n");
        exit(1);
    }
    N = atoi(argv[1]);
    if(N <= 0){
        fprintf(stderr, "%s", "Error: Invalid matrix size\n");
        exit(1);
    }
    double** A = matGeneratev1();

    //Wall clock time for the checkSym function
    struct timespec start_checkSym, end_checkSym;
    if (clock_gettime(CLOCK_MONOTONIC, &start_checkSym) == -1) {
        perror("clock_gettime");
        return 1;
    }
    int isSym = checkSym(A);
    if (clock_gettime(CLOCK_MONOTONIC, &end_checkSym) == -1) {
        perror("clock_gettime");
        return 1;
    }
    double checkSym_time = (end_checkSym.tv_sec - start_checkSym.tv_sec) + (end_checkSym.tv_nsec - start_checkSym.tv_nsec) / 1e9;
    printf("checkSym time: %f\n", checkSym_time);
    if (isSym) {
        for (int i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
        return 0;
    }

    //Allocate memory for the transposed matrix
    double** B = (double**) malloc(N * sizeof(double*));
    if (B == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        B[i] = (double*) malloc(N * sizeof(double));
        if (B == NULL){
            printf("Memory allocation failed\n");
            exit(1);
        }   
    }

    //Wall clock time for the matTranspose function
    struct timespec start_matTranspose, end_matTranspose;
    if (clock_gettime(CLOCK_MONOTONIC, &start_matTranspose) == -1) {
        perror("clock_gettime");
        return 1;
    }
    matTranspose(A, B);
    if (clock_gettime(CLOCK_MONOTONIC, &end_matTranspose) == -1) {
        perror("clock_gettime");
        return 1;
    }
    double matTranspose_time = (end_matTranspose.tv_sec - start_matTranspose.tv_sec) + (end_matTranspose.tv_nsec - start_matTranspose.tv_nsec) / 1e9;
    printf("matTranspose time: %f\n", matTranspose_time);

    //Cleanup
    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    return 0;
}   