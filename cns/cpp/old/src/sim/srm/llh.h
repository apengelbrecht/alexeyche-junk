#ifndef LLH_H
#define LLH_H

//#include "neurons.h"

#include <sim/core.h>

namespace srm {
    class TLogLikelyhood {
    public:
        TLogLikelyhood(SrmNeuron *nv, double T0v, double Tmaxv) : n(nv), T0(T0v), Tmax(Tmaxv) { if(T0<0) T0 = 0;}
        vec grad(); 

        SrmNeuron *n;
        double T0;
        double Tmax;
    };
};

#endif