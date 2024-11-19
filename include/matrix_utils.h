#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

double** matGenerate(int N);
double** matGenerateSym(int N);
int checkSymSeq(double** A, int N);
int checkSymImplicitPar(double** A, int N);
int checkSymExplicitPar(double** A, int N);

#endif