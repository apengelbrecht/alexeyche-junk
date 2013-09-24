#!/usr/bin/env python


from brian import *
import numpy

#startTime = 0*second
#endTime = 20*second

def run_poisson(eqs, start_val, sec):
    group = NeuronGroup(1, model=eqs, threshold=PoissonThreshold('x'))
    group.x = start_val
    M = SpikeMonitor(group)
    Ms = StateMonitor(group, 'x', record=True)
    run(sec*second)
    return Ms[0], M.spiketimes[0]

def generate_lowtrend_proc():
    eqs = Equations('dx/dt=k/tau : Hz', k=0*Hz, tau=1*ms)
    state, sp = run_poisson(eqs, 15, 2)
    eqs = Equations('dx/dt=k/tau : Hz', k=-0.001*Hz, tau=1*ms)
    state2, sp2 = run_poisson(eqs, state[-1], 10)
    eqs = Equations('dx/dt=k/tau : Hz', k=0*Hz, tau=1*ms)
    state3, sp3 = run_poisson(eqs, state2[-1], 5)
    return numpy.concatenate((state,state2,state3)), numpy.concatenate((sp,sp2,sp3))

state, sp = generate_lowtrend_proc()
