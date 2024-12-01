#ifndef OMP_PAR_H
#define OMP_PAR_H

int checkSymOMP(double* M, int numThreads);
int checkSymOMPTime(double* M, int numThreads, double* time);
double* matTransposeOMP(double* M, int blockSize, int numThreads, double* time);
void transposeRecursive(double* M, double* T, int i, int j, int dim, int blockSize);
double* matTransposeOMPCacheOblivious(double* M, int blockSize, double* time);
#endif