
#include "matrix.h"

#include <templates_clean.h>
#define T pMatrix
#define DESTRUCT deleteMatrix
#include <util/util_vector_tmpl.c>



Matrix *createMatrix(size_t nr, size_t nc) {
    Matrix *m = (Matrix*)malloc(sizeof(Matrix));
    m->nrow = nr;
    m->ncol = nc;
    m->vals = (double*)malloc(m->nrow*m->ncol*sizeof(double));
    return(m);
}

void deleteMatrix(Matrix *m) {
    free(m->vals);
    free(m);
}

void setMatrixElement(Matrix *m, size_t i, size_t j, double val) {
    m->vals[j*m->nrow + i] = val;
}
double getMatrixElement(Matrix *m, size_t i, size_t j) {
    return m->vals[j*m->nrow + i];
}

void printMatrix(Matrix *m) {
    for(size_t mi=0; mi < m->nrow; mi++) {
        for(size_t mj=0; mj < m->ncol; mj++) {
            printf("%f ", getMatrixElement(m, mi, mj));
        }
        printf("\n");
    }
}



