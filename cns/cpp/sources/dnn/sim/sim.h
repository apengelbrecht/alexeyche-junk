#pragma once

#include <dnn/base/factory.h>
#include <dnn/util/spinning_barrier.h>

#include "builder.h"
#include "network.h"

namespace dnn {

class Sim {
public:
	Sim(const Constants &_c) : c(_c), duration(0.0) {
	}
	void build(Stream* input_stream = nullptr) {
		Builder b(c);
		if(input_stream) {
			b.setInputModelStream(input_stream);
		}
		neurons = b.buildNeurons();			
		for(auto &n: neurons) {
			if(n.ref().inputIsSet()) {
				duration = std::max(duration, n.ref().getInput().getDuration());
			}
		}
		net = uptr<Network>(new Network(neurons));
	}
	
	void serialize(Stream &output_stream) {
		for(auto &n : neurons) {
			output_stream.writeObject(n.ptr());
		}
	}
	void saveStat(Stream &str) {
		for(auto &n: neurons) {
			if(n.ref().stat.on()) {
				str.writeObject(&n.ref().stat); 
			}
		}
	}
	
	
	static void runWorker(Sim &s, size_t from, size_t to, SpinningBarrier &barrier) {
		Time t(s.c.sim_conf.dt);

		for(; t<s.duration; ++t) {
			for(size_t i=from; i<to; ++i) {
				
				s.neurons[i].ifc().calculateDynamics(t);
				if(s.neurons[i].ifc().pullFiring()) {
					cout << s.neurons[i].ref().id() << " made spike\n";
				}
			}
			barrier.wait();
		}		
		
	}
	void run(size_t jobs) {
		vector<IndexSlice> slices = dispatchOnThreads(neurons.size(), jobs);
		vector<std::thread> threads;
		
		SpinningBarrier barrier(jobs);		
		for(auto &slice: slices) {
			threads.emplace_back(Sim::runWorker, std::ref(*this), slice.from, slice.to, std::ref(barrier));
		}
		for(auto &t: threads) {
			t.join();
		}
	}


private:
	double duration;
	const Constants &c;
	vector<InterfacedPtr<SpikeNeuronBase>> neurons;
	uptr<Network> net;
};

}
