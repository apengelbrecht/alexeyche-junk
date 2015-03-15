


#include <dnn/base/base.h>
#include <dnn/neurons/leaky_integrate_and_fire.h>
#include <dnn/act_functions/determ.h>
#include <dnn/synapses/static_synapse.h>
#include <dnn/inputs/input_time_series.h>
#include <dnn/io/serialize.h>
#include <dnn/util/time_series.h>

#include "factory.h"

namespace dnn {

Factory::entity_map_type Factory::typemap;
Factory::proto_map_type Factory::prototypemap;

Factory& Factory::inst() {
	static Factory _inst;
	return _inst;
}


#define REG_TYPE(name) \
	registerType<name>(#name);\

#define REG_TYPE_WITH_CONST(name) \
	registerType<name>(#name);\
	registerType<name##C>(string(#name) + string("C"));\

#define REG_TYPE_WITH_STATE_AND_CONST(name) \
	registerType<name>(#name);\
	registerType<name##C>(string(#name) + string("C"));\
	registerType<name##State>(string(#name) + string("State"));\


Factory::Factory() {
	REG_TYPE_WITH_STATE_AND_CONST(LeakyIntegrateAndFire);
	REG_TYPE(SpikeNeuronInfo);
	REG_TYPE_WITH_STATE_AND_CONST(StaticSynapse);
	REG_TYPE(SynapseInfo);
	REG_TYPE_WITH_CONST(Determ);
	REG_TYPE_WITH_STATE_AND_CONST(InputTimeSeries);

}

Factory::~Factory() {
	for (auto &o : objects) {
		delete o;
	}
}

void Factory::deleteLast() {
	auto p = objects_map.equal_range(objects.back()->name());
	for (auto it = p.first; it != p.second; ++it) {
		if (it->second == objects.size()-1) { 
			objects_map.erase(it);
		}
	}
	delete objects.back();
	objects.pop_back();
}

SerializableBase* Factory::createObject(string name) {
	if (typemap.find(name) == typemap.end()) {
		cerr << "Failed to find method to construct type " << name << "\n";
		terminate();
	}
	SerializableBase* o = typemap[name]();
	objects.push_back(o);
	objects_map.insert(std::make_pair(o->name(), objects.size()-1));
	return o;
}

ProtoMessage Factory::createProto(string name) {
	if (prototypemap.find(name) == prototypemap.end()) {
		cerr << "Failed to find method to construct proto type " << name << "\n";
		terminate();
	}
	ProtoMessage o = prototypemap[name]();
	return o;
}


SpikeNeuronBase* Factory::createSpikeNeuron(string name) {
	SerializableBase *b = createObject(name);
	SpikeNeuronBase *p = dynamic_cast<SpikeNeuronBase*>(b);
	if (!p) {
		cerr << "Error to cast " << b->name() << " to SpikeNeuronBase" << "\n";
		terminate();
	}
	return p;
}

ActFunctionBase* Factory::createActFunction(string name) {
	SerializableBase *b = createObject(name);
	ActFunctionBase *p = dynamic_cast<ActFunctionBase*>(b);
	if (!p) {
		cerr << "Error to cast " << b->name() << " to ActFunctionBase" << "\n";
		terminate();
	}
	return p;
}

TimeSeries* Factory::getCachedTimeSeries(const string &name, const string& filename, const string& format) {
    if(ts_map.find(name) == ts_map.end()) {
        ts_map[name] = new TimeSeries(filename, format);
        objects.push_back(ts_map[name]);
    }
    return ts_map[name];
}

pair<Factory::object_iter, Factory::object_iter> Factory::getObjectsSlice(const string& name) {
	return objects_map.equal_range(name);
}

}
