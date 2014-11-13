#pragma once

static size_t global_neuron_index = 0;

#include "synapse.h"

class Neuron: public Printable {
public:
    Neuron() {}

};


class SRMNeuron : public Neuron {
public:
    SRMNeuron(size_t _id) : id(_id), y(0.0) {}
    size_t id;
    double y;
    vector<Synapse> syns;

    void print(std::ostream& str) const {
        str << "SRMNeuron(" << id << ")\n";
        str << "\ty == " << y;
        str << "\tsynapses: \n";
        for(auto it=syns.begin(); it != syns.end(); ++it) {
            str << *it << ",";
        }
    }
};

