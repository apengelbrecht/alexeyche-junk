
#include "sim.h"

Sim* createSim() {
   Sim *s = (Sim*) malloc(sizeof(Sim));
   s->layers = TEMPLATE(createVector,pSRMLayer)(); 
   s->n.net_size = 0;
}


void appendLayerSim(Sim *s, SRMLayer *l) {
    TEMPLATE(insertVector,pSRMLayer)(s->layers, l);
}

void freeSimNet(Net *n) {
    if(n->net_size > 0) {
        printf("net_size: %zu\n", n->net_size);

        for(size_t net_i=0; net_i < n->net_size; net_i++) {
            free(n->net[net_i]);
        }
        free(n->net_lens);
    }
}

void deleteSim(Sim *s) {
    for(size_t li=0; li< s->layers->size; li++) {
        deleteSRMLayer(s->layers->array[li]);
    }
    TEMPLATE(deleteVector,pSRMLayer)(s->layers);
    freeSimNet(&s->n);
    free(s);
}


void configureSim(Sim *s, Constants *c) {
    indVector *input_ids = TEMPLATE(createVector,ind)();
    for(size_t inp_i=0; inp_i<c->M; inp_i++) {
        TEMPLATE(insertVector,ind)(input_ids, inp_i);
    }
    freeSimNet(&s->n);
    s->n.net_size += c->M;
    
    size_t neurons_idx = input_ids->size; // start from last input ID
    for(size_t li=0; li< c->layers_size->size; li++) {
        SRMLayer *l = createSRMLayer(c->layers_size->array[li], &neurons_idx);
        appendLayerSim(s, l);
        s->n.net_size += l->N;
    }   
    s->n.net = (double**) malloc(s->n.net_size * sizeof(double*));
    s->n.net_lens = (size_t*) malloc(s->n.net_size * sizeof(size_t)); 
    for(size_t net_i=0; net_i < s->n.net_size; net_i++) {
        s->n.net_lens[net_i] = 0;
    }
    
    TEMPLATE(deleteVector,ind)(input_ids);
}
