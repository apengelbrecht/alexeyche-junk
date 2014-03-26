#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>

#include "constants.h"
#include "core.h"

typedef enum {EXC, INH} nspec_t;

typedef struct {
    size_t N;
    size_t *ids;
    nspec_t *nt;

    double **W;
    double **syn;
    double **syn_spec;
    size_t **id_conns;
    int *nconn;
} SRMLayer;

typedef SRMLayer* pSRMLayer;

#ifdef T
#undef T
#endif
#define T pSRMLayer
#include <util/util_vector_tmpl.h>


SRMLayer* createSRMLayer(size_t N, size_t *glob_idx);
void printSRMLayer(SRMLayer *l);
void deleteSRMLayer(SRMLayer *l);
void configureSRMLayer(SRMLayer *l, const indVector *inputIDs, Constants *c);
nspec_t getSpecNeuron(SRMLayer *l, const size_t *id);

//void connectSRMLayer(SRMLayer *l, Matrix *conn_m, indVector *ids_conn, Constants *c);

#endif