#ifndef SEQ_H
#define SEQ_H

double** matGenerate(int N);
double** matGenerateSym(int N);
int checkSymSeq(double** A, int N, double* time);
double** matTranspose(double** M, int N, double* time);
int isTransposed(double** A, double** B, int N);

#endif