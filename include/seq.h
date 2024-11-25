#ifndef SEQ_H
#define SEQ_H

double* matGenerate();
double* matGenerateSym();
void matRandomize(double* M);
int checkSymSeq(double* A, double* time);
double* matTranspose(double* M, double* time);
int isTransposed(double* A, double* B);

#endif