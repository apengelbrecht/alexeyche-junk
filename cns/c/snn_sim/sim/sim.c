
#include "sim.h"

Sim* createSim(size_t nthreads, unsigned char stat_level, Constants *c) {
    Sim *s = (Sim*) malloc(sizeof(Sim));
    s->ns = createNetSim();
    s->layers = TEMPLATE(createVector,pLayer)(); 
    s->rt = createRuntime();
    s->ctx = (SimContext*) malloc(sizeof(SimContext));
    s->ctx->c = c;
    s->ctx->global_reward = 0;
    s->ctx->mean_global_reward = 0;
    s->ctx->stat_level = stat_level;
    if(s->ctx->stat_level > 0) {
        s->ctx->stat_global_reward = TEMPLATE(createVector,double)();
    }
    s->impl = (SimImpl*) malloc(sizeof(SimImpl));
    s->impl->nthreads = nthreads;
    return(s);
}


void appendLayerSim(Sim *s, Layer *l) {
    TEMPLATE(insertVector,pLayer)(s->layers, l);
    l->id = s->layers->size - 1;
}

void deleteSim(Sim *s) {
    TEMPLATE(deleteVector,pLayer)(s->layers);
    deleteNetSim(s->ns);
    deleteRuntime(s->rt);
    if(s->ctx->stat_level > 1) {
        TEMPLATE(deleteVector,double)(s->ctx->stat_global_reward);
    }
    free(s->ctx);
    free(s->impl);
    free(s);
}

size_t getLayerIdOfNeuron(Sim *s, size_t n_id) {
    for(size_t li=0; li<s->layers->size; li++) {
        Layer *l = s->layers->array[li];
        assert(l->N);
        if((l->ids[0] <= n_id) && (l->ids[l->N-1] >= n_id)) {
            return(l->id);
        }
    }
    printf("Can't find layer id for neuron with id %zu\n", n_id);
    exit(1);
}


const SynSpike* getInputSpike(double t, const size_t *n_id, NetSim *ns, SimRuntime *sr, const Constants *c) {
    if(ns->input_spikes_queue[ *n_id ]->current != NULL) {
        const SynSpike *sp = &ns->input_spikes_queue[ *n_id ]->current->value;
        if(sp->t - t < 0) {
            printf("We missing an input spike %f in %zu at %f. Something wrong (%f). Need sorted queue\n", sp->t, *n_id, t, sp->t - t);
            exit(1);
        }
        if(sp->t <= t+c->dt) {
            ns->input_spikes_queue[ *n_id ]->current = ns->input_spikes_queue[ *n_id ]->current->next;
            return(sp);
        }
    }
    pthread_spin_lock(&spinlocks[ *n_id ]);
    if(ns->spikes_queue[ *n_id ]->current != NULL) {
        const SynSpike *sp = &ns->spikes_queue[ *n_id ]->current->value;
        if(sp->t - t < 0) {
            printf("We missing net spike %f in %zu at %f. Something wrong (%f). Need sorted queue\n", sp->t, *n_id, t, sp->t - t);
            exit(1);
        }
        if(sp->t <= t+c->dt) {
            ns->spikes_queue[ *n_id ]->current = ns->spikes_queue[ *n_id ]->current->next;
            pthread_spin_unlock(&spinlocks[*n_id]);
            return(sp);
        }
    }
    pthread_spin_unlock(&spinlocks[*n_id]);
    return(NULL);
}



void runSim(Sim *s) {
    configureSimAttr(s->impl);
    if(s->ctx->c->reinforcement) {
        configureRewardModulation(s);
//        global_reward_spinlock = (pthread_spinlock_t*)malloc( sizeof(pthread_spinlock_t));
//        pthread_spin_init(global_reward_spinlock, 0);
    }

    spinlocks = (pthread_spinlock_t*)malloc( s->impl->net_size * sizeof(pthread_spinlock_t));
    for(size_t ni=0; ni<s->impl->net_size; ni++) {
        pthread_spin_init(&spinlocks[ni], 0); // net sim spinlock
    }
    
    pthread_t *threads = (pthread_t *) malloc( s->impl->nthreads * sizeof( pthread_t ) );
    SimWorker *workers = (SimWorker*) malloc( s->impl->nthreads * sizeof(SimWorker) );
    for(size_t ti=0; ti < s->impl->nthreads; ti++) {
        workers[ti].thread_id = ti;
        workers[ti].s = s;
        int neuron_per_thread = (s->impl->num_neurons + s->impl->nthreads - 1) / s->impl->nthreads;
        workers[ti].first = min( ti    * neuron_per_thread, s->impl->num_neurons );
        workers[ti].last  = min( (ti+1) * neuron_per_thread, s->impl->num_neurons );
    }
    
    pthread_attr_t attr;
    P( pthread_attr_init( &attr ) );
    P( pthread_barrier_init( &barrier, NULL, s->impl->nthreads ) );
    for( int i = 1; i < s->impl->nthreads; i++ )  {
        P( pthread_create( &threads[i], &attr, simRunRoutine,  &workers[i]) );

    }
    simRunRoutine(&workers[0]);

    free(workers);
    free(threads);
    for(size_t ni=0; ni<s->impl->net_size; ni++) {
        pthread_spin_destroy(&spinlocks[ni]);
    }
//    if(s->c->reinforcement) {
//        pthread_spin_destroy(global_reward_spinlock);
//    }
}

void* simRunRoutine(void *args) {
    SimWorker *sw = (SimWorker*)args;
    Sim *s = sw->s;
    SimContext *ctx = s->ctx;
    const Constants *c = ctx->c; 
    SimImpl* impl = s->impl;

    for(ctx->t=0; t< s->rt->Tmax; ctx->t+=c->dt) {
//        printf("t: %f\n",t);
        for(size_t na_i=sw->first; na_i<sw->last; na_i++) {
            simulateNeuron(s, &impl->na[na_i].layer_id, &impl->na[na_i].n_id, t, c);
        }
        //bool we_did_reset = false;
        //if(s->rt->reset_timeline->size > s->rt->timeline_iter) {
        //    if(s->rt->reset_timeline->array[ s->rt->timeline_iter ] <= t) {
        //        we_did_reset = true;
        //        for(size_t na_i=first; na_i<last; na_i++) {
        //            SRMLayer *l = s->layers->array[s->na[na_i].layer_id];
        //            resetSRMLayerNeuron(l, &s->na[na_i].n_id);
        //        }

        //    }
        //}
        pthread_barrier_wait( &barrier );
        //if(we_did_reset) {
        //    if(sw->thread_id == 0) {
        //        ++s->rt->timeline_iter;
        //    }
        //    pthread_barrier_wait( &barrier );
        //}
        if(s->c->reinforcement) { 
            if(sw->thread_id == 0) {
                double B_acc = 0;

                for(size_t ni=0; ni < s->layers->array[ s->layers->size-1]->N; ni++) {
                    if( s->c->learning_rule == EOptimalSTDP) {
                        B_acc += ((TOptimalSTDP*) (s->layers->array[ s->layers->size-1 ]->ls_t))->B[ni];
                        ((TOptimalSTDP*) (s->layers->array[ s->layers->size-1 ]->ls_t))->B[ni] = 0;
                    } else
                    if( s->c->learning_rule == EResourceSTDP) {
                        B_acc += ((TResourceSTDP*) (s->layers->array[ s->layers->size-1 ]->ls_t))->reward[ni];
                        ((TResourceSTDP*) (s->layers->array[ s->layers->size-1 ]->ls_t))->reward[ni] = 0;
                    } 
                }
                s->global_reward = B_acc/s->layers->array[ s->layers->size-1]->N;
                s->mean_global_reward += s->global_reward;

                s->mean_global_reward -= s->mean_global_reward/s->c->trew;
                if((s->stat_level > 0)&&(B_acc != 0)) {
                    TEMPLATE(insertVector,double)(s->stat_global_reward, B_acc);
                }
            }
            pthread_barrier_wait( &barrier );
        }
    }
    return(NULL);
}

void simulateNeuron(Sim *s, const size_t *layer_id, const size_t *n_id, double t,  const Constants *c) {
    const SynSpike *sp;
    SRMLayer *l = s->layers->array[*layer_id];
    while( (sp = getInputSpike(t, &l->ids[*n_id], s->ns, s->rt,  c)) != NULL) {
//        printf("got input spike on %zu:%zu at %f in syn %zu (%f)\n", *layer_id, *n_id, t, sp->syn_id, sp->t);
        propagateSpikeSRMLayer(l, n_id, sp, c);
    }
    simulateSRMLayerNeuron(l, n_id, s, &t);
    if(l->fired[*n_id] == 1) {
        propagateSpikeNetSim(s, l, &l->ids[*n_id], t);
        l->fired[*n_id] = 0;
    }
}


