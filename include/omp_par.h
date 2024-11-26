#ifndef OMP_PAR_H
#define OMP_PAR_H

int checkSymOMP(double* A, int blockSize, int numThreads);
int checkSymOMPTime(double* A, int blockSize, int numThreads, double* time);
double* matTransposeOMP(double* A, int blockSize, int numThreads, double* time);

#endif