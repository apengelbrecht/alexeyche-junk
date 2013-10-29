
#include <sim/core.h>

#include "srm.h"
#include "srm_neurons.h"

namespace srm {

    double SrmNeuron::u(const double &t) {
        double epsp_pot = 0;
        double nu_pot = 0;
        for(size_t i=0; i<in.size(); i++) {
            for(size_t j=0; j< in[i]->y.n_elem(t); j++) {
    //                    Log::Info << "epsp_pot: " << epsp_pot; 
    //                    Log::Info << " w:" << w[i] << " t:"  << t << " in.y(j):" << in[i]->y(j) << " y:"  << y.last() << "\n";
                epsp_pot += w(i)*srm::epsp(t, in[i]->y(j), y.last());
            }
        }
        for(size_t i=0; i<y.n_elem(t); i++) {
            nu_pot += srm::nu(t, y(i));
        }
        return u_rest + epsp_pot + nu_pot;
    }
        
    double SrmNeuron::p(const double &t) {
        Log::Info << "u(t) = " << u(t) << "\n";
        double uc = u(t);
        return (beta/alpha)*(log(1+exp(alpha*(tresh-uc))) - alpha*(tresh-uc));
    }

    double prob(SrmNeuron *n) {
        Log::Info << "Probabilites: \n";
        for(size_t yi=0; yi<n->y.size(); yi++) {
            Log::Info << " at " << n->y(yi) << " : ";
            Log::Info << n->p(n->y(yi)) << "\n";
        }
    }  

};        
