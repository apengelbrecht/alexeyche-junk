#pragma once


#include <snnlib/layers/layer.h>
#include <snnlib/util/time_series.h>


#include "network.h"

#define P( condition ) {if( (condition) != 0 ) { printf( "\n FAILURE in %s, line %d\n", __FILE__, __LINE__ );exit( 1 );}}


typedef NeuronAddress pair<size_t, size_t>;


class Sim: public Printable {
public:
    Sim(const Constants &c, size_t _jobs=1);
    
    struct SimWorker {
        Sim *s;
        size_t thread_id;
        int first;
        int last;
    };

    ~Sim() {
        if(!statistics_file.empty()) {
            ProtoRw prw(statistics_file, ProtoRw::Write);
            for(auto it=sc.neurons_to_listen.begin(); it != sc.neurons_to_listen.end(); ++it) {
                Neuron *n = accessByGlobalId(*it);
                Serializable *st = n->getStat();
                if(st) {
                    prw.write(st);
                }
            }
        }
        if(!output_spikes_file.empty()){
            ProtoRw prw(output_spikes_file, ProtoRw::Write);
            prw.write(&net.spikes_list);
        }

        layers.clear();
    }

    void print(std::ostream& str) const {
        for(auto it=layers.begin(); it!=layers.end(); ++it) {
            str << **it;
        }
        str << net;
    }
    void setInputTimeSeries(LabeledTimeSeriesList l) {
        input_ts = ContLabeledTimeSeries(l, sc.ts_map_conf.dt);
    }
    void setOutputSpikesFile(const string &filename) {
        output_spikes_file = filename;
    }

    Neuron* accessByGlobalId(size_t id) {
        size_t acc = 0;
        for(size_t li=0; li<layers.size(); li++) {
            Layer *l = layers[li];
            if(id<(acc+l->N)) {
                return layers[li]->neurons[id-acc];
            } else {
                acc += l->N;
            }
        }
        cerr << "Can't find neuron with global id " << id << "\n";
        terminate();
    }


    void monitorStat(const string &filename) {
        statistics_file = filename;
        for(auto it=sc.neurons_to_listen.begin(); it != sc.neurons_to_listen.end(); ++it) {
            accessByGlobalId(*it)->enableCollectStatistics();
        }
    }
    void precalculateInputLayerSpikes();
   
    static void* runWorker(void *context);
    void run();

    size_t jobs;
    Network net;

    ContLabeledTimeSeries input_ts;

    size_t input_layers_count;
    
    size_t input_neurons_count;
    size_t net_neurons_count;

    vector< Layer *> layers;
    vector<NeuronAddress> na;

    string statistics_file;
    string output_spikes_file;

    const SimConfiguration &sc;

    RuntimeGlobals rg;
};

