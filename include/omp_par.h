#ifndef OMP_PAR_H
#define OMP_PAR_H

int checkSymOMP(double* A, int blockSize);
int checkSymOMPTime(double* A, int blockSize, double* time);
double* matTransposeOMP(double* A, int blockSize, double* time);

#endif