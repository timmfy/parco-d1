#ifndef IMP_PAR_H
#define IMP_PAR_H

int checkSymImp(double* A, int blockSize);
int checkSymImpTime(double* A, int blockSize, double* time);
double* matTransposeImp(double* A, int blockSize, double* time);

#endif