#pragma once

#include "base.h"

namespace dnn {


template <typename State, typename Eval = double, typename InputEval = double, typename SynapseSystemEval = double>
class Neuron : public DynamicObject<State, Eval> {
public:
    Neuron(EvalObject<InputEval> &i, EvalObject<SynapseSystemEval> &syn) : input(i), synapses(syn) {}

    EvalObject<InputEval> &input;
    EvalObject<SynapseSystemEval> &synapses;
};



}
