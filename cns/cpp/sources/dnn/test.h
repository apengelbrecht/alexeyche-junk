#pragma once

#include "base.h"
#include "neuron.h"
#include "synapse.h"

namespace dnn {

class InputTimeSeriesState : public BaseState<1> {};

class InputTimeSeries : public DynamicObject<InputTimeSeriesState, double> {
public:
    double eval() {
        return 5;
    }    
    void step(InputTimeSeriesState &dState_dt) {

    }
};


class LeakyIntegrateAndFireState : public BaseState<1> {
};

class LeakyIntegrateAndFire : public Neuron<LeakyIntegrateAndFireState> {
public:    
    LeakyIntegrateAndFire(EvalObject<double> &inp, EvalObject<double> &syn) : Neuron(inp, syn) {}
    double eval() {
        return 2*input.eval() + synapses.eval();
    }
    void step(LeakyIntegrateAndFireState &dState_dt) {

    }
};

}

void test();
