#ifndef IMP_PAR_H
#define IMP_PAR_H

int checkSymImp(double** A, int N, int blockSize);
int checkSymImpTime(double** A, int N, int blockSize);
double** matTransposeImp(double** A, int N, int blockSize);
#endif