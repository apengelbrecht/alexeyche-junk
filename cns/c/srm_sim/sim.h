#ifndef SIM_H
#define SIM_H

#include <stddef.h>

#include <layer.h>
#include <constants.h>
#include <spikes_list.h>
#include <matrix.h>
#include <io.h>

#include <pthread.h>

#ifndef max
    #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
    #define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif
#define P( condition ) {if( (condition) != 0 ) { printf( "\n FAILURE in %s, line %d\n", __FILE__, __LINE__ );exit( 1 );}}

#include <sim_runtime.h>
#include <sim_netsim.h>



pthread_barrier_t barrier;

typedef struct {
    size_t layer_id;
    size_t n_id;
} NeuronAddress;

typedef struct {
    pSRMLayerVector *layers;
    Constants *c;
    
    SimRuntime *rt;
    NetSim *ns;
    size_t net_size;
    
    size_t nthreads;
    NeuronAddress *na;
    size_t num_neurons;
} Sim;

typedef struct {
    Sim *s;
    size_t thread_id;
} SimWorker;



NetSim* createNetSim();
void deallocNetSim(NetSim *ns);
void allocNetSim(NetSim *ns, size_t net_size);
void deleteNetSim(NetSim *ns);
void propagateInputSpikesNetSim(Sim *s, SpikesList *sl);
void printInputSpikesQueue(NetSim *ns);
void configureConnMapNetSim(NetSim *ns, pSRMLayerVector *l);
void propagateSpikeNetSim(Sim *s, SRMLayer *l, const size_t *ni, double t);


Sim* createSim();
void appendLayerSim(Sim *s, SRMLayer *l);
// configure
void deleteSim(Sim *s);
void configureLayersSim(Sim *s, Constants *c, bool saveStat);
SimRuntime* createRuntime();
void deleteRuntime(SimRuntime *sr);
void configreNetSpikesSim(Sim *s, Constants *c);
void configureSimAttr(Sim *s);

// serialize
void loadLayersFromFile(Sim *s, const char *model_fname, Constants *c, bool saveStat);
void saveLayersToFile(Sim *s, const char *model_file);

// run
void runSim(Sim *s);
void simulateNeuron(Sim *s, const size_t *layer_id, const size_t *n_id, double t,  const Constants *c);
const SynSpike* getInputSpike(double t, const size_t *n_id, NetSim *ns, SimRuntime *sr, const Constants *c);
void* simRunRoutine(void *args);



#endif
