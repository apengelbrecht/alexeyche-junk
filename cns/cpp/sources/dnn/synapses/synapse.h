#pragma once

#include <dnn/io/serialize.h>
#include <dnn/util/statistics.h>

namespace dnn {


struct SynapseInterface {
	stateDelegate propagateSpike;
	calculateDynamicsDelegate calculateDynamics;
	retDoubleDelegate getMembranePotential;
};

class Network;
class SpikeNeuronBase;
class Builder;

class SynapseBase : public SerializableBase {
friend class Network;
friend class SpikeNeuronBase;
friend class Builder;
public:
	SynapseBase() : _fired(false) {}
	typedef SynapseInterface interface;

	inline const size_t& idPre() { 
		return _id_pre;
	}
	inline size_t& mutIdPre() { return _id_pre; }

	virtual void propagateSpike() = 0;
	virtual double getMembranePotential() = 0;
	virtual void calculateDynamics(const Time &t) = 0;
	virtual void reset() = 0;
		
	template <typename T>
	void provideInterface(SynapseInterface &i) {
        i.propagateSpike = MakeDelegate(static_cast<T*>(this), &T::propagateSpike);
        i.calculateDynamics = MakeDelegate(static_cast<T*>(this), &T::calculateDynamics);
        i.getMembranePotential = MakeDelegate(static_cast<T*>(this), &T::getMembranePotential);
    }

	static void provideDefaultInterface(SynapseInterface &i) {
		throw dnnException() << "There is no default implementation for synapse\n";
	}
	Statistics& getStat() {
		return stat; 
	}
	inline void setFired(bool fired) {
		_fired = fired;
	}
	inline const double fired() const {
		return _fired;
	}
	inline double& mutWeight() {
		return _weight;
	}
	inline const double& weight() {
		return _weight;
	}
protected:
	size_t _id_pre;
	double dendrite_delay;
	double _weight;
	bool _fired;

	Statistics stat;
};


/*@GENERATE_PROTO@*/
struct SynapseInfo : public Serializable<Protos::SynapseInfo> {
	void serial_process() {
		begin() << "id_pre: " 		  << id_pre 		<< ", " \
		        << "dendrite_delay: " << dendrite_delay << ", " \
		        << "weight: " 		  << weight 		<< Self::end;
	}
	size_t id_pre;
	double dendrite_delay;
	double weight;
};



template <typename Constants, typename State>
class Synapse : public SynapseBase {

public:	
	SynapseInfo getInfo() {
		SynapseInfo info;
		info.id_pre = _id_pre;
		info.dendrite_delay = dendrite_delay;
		info.weight = _weight;
		return info;
	}

	void serial_process() {
		begin() << "Constants: " << c;

		if (messages->size() == 0) {
			(*this) << Self::end;
			return;
		}

		(*this) << "State: " << s;

		if (messages->size() == 0) {
			(*this) << Self::end;
			return;
		}
		SynapseInfo info;
		if (mode == ProcessingOutput) {
			info = getInfo();
		}

		(*this) << "SynapseInfo: "   << info;
		(*this) << Self::end;
	}
protected:
	State s;
	Constants c;
};


}
