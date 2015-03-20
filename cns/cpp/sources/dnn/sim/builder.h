#pragma once



namespace dnn {


class Builder {
public:
	struct Layer {
		vector<InterfacedPtr<SpikeNeuronBase>> neurons;
	};



	Builder(const Constants &_c) : c(_c), input_stream(nullptr) {

	}


	vector<InterfacedPtr<SpikeNeuronBase>> buildNeurons() {
		vector<Layer> layers;
		for (const string &lc : c.sim_conf.layers) {
			Layer layer;
			Document layer_conf = Json::parseStringC(lc);
			for (size_t ni = 0; ni < Json::getUintVal(layer_conf, "size"); ++ni) {
				InterfacedPtr<SpikeNeuronBase> n;

				if (input_stream) {
					n.set(input_stream->readObject<SpikeNeuronBase>());
				} else {
					n.set(buildObjectFromConstants<SpikeNeuronBase>(Json::getStringVal(layer_conf, "neuron"), c.neurons));
					n.ref().axon_delay = Json::getDoubleValDef(layer_conf, "axon_delay", 0.0);

					const string act_function = Json::getStringValDef(layer_conf, "act_function", "");
					if (!act_function.empty()) {
						n.ref().setActFunction(buildObjectFromConstants<ActFunctionBase>(act_function, c.act_functions));
					}
					const string input = Json::getStringValDef(layer_conf, "input", "");
					if (!input.empty()) {
						n.ref().setInput(buildObjectFromConstants<InputBase>(input, c.inputs));
					}
				}
				layer.neurons.push_back(n);
			}
			layers.push_back(layer);
		}
		for (auto it = c.sim_conf.files.begin(); it != c.sim_conf.files.end(); ++it) {
			const string &obj_name = it->first;
			Document file_conf = Json::parseStringC(it->second);
			
			auto p = Factory::inst().getObjectsSlice(obj_name);
			for (auto oit = p.first; oit != p.second; ++oit) {
				SerializableBase *o = Factory::inst().getObject(oit);
				InputBase* inp = dynamic_cast<InputBase*>(o);
				if (!inp) {
					throw dnnException()<< "Failed to set input file for " << o->name() << "\n";
				}
				if(!fileExists(Json::getStringVal(file_conf, "filename"))) {
					continue;
				}
				TimeSeries &ts = Factory::inst().getCachedTimeSeries(
					Json::getStringVal(file_conf, "filename"),
					Json::getStringVal(file_conf, "format")
				);
				inp->setTimeSeries(&ts);
			}
		}
		if (!input_stream) {
			for (auto it = c.sim_conf.conn_map.begin(); it != c.sim_conf.conn_map.end(); ++it) {
				size_t l_id_pre = it->first.first;
				size_t l_id_post = it->first.second;
				const string conn_conf = it->second;
				if (l_id_pre >= layers.size()) {
					throw dnnException()<< "Can't find layer with id " << l_id_pre << "\n";
				}
				if (l_id_post >= layers.size()) {
					throw dnnException()<< "Can't find layer with id " << l_id_post << "\n";
				}
				connectLayers(layers[l_id_pre], layers[l_id_post], conn_conf);
			}
		}

		vector<InterfacedPtr<SpikeNeuronBase>> neurons;
		for (auto &l : layers) {
			neurons.insert(neurons.end(), l.neurons.begin(), l.neurons.end());
		}
		turnOnStatistics(neurons, c.sim_conf.neurons_to_listen);
		return neurons;
	}

	static void turnOnStatistics(vector<InterfacedPtr<SpikeNeuronBase>> &neurons, const vector<size_t> &ids) {
		for(auto it=ids.cbegin(); it != ids.cend(); ++it) {
			neurons[*it].ref().stat.turnOn();
			for(auto s: neurons[*it].ref().getSynapses()) {
				s.ref().stat.turnOn();
			}
		}
	}
	void connectLayers(Layer &pre, Layer &post, const string &conn_conf_s) {
		Document conn_conf = Json::parseStringC(conn_conf_s);
		for (auto &npre : pre.neurons) {
			for (auto &npost : post.neurons) {
				if (getUnif() < Json::getDoubleValDef(conn_conf, "prob", 1.0)) {
					InterfacedPtr<SynapseBase> syn(
					    buildObjectFromConstants<SynapseBase>(Json::getStringVal(conn_conf, "synapse"), c.synapses)
					);
					syn.ref().id_pre = npre.ref().id();
					syn.ref().dendrite_delay = Json::getDoubleValDef(conn_conf, "dendrite_delay", 0.0);
					syn.ref().weight = Json::getDoubleVal(conn_conf, "start_weight");
					npost.ref().addSynapse(syn);
				}
			}
		}
	}

	template <typename T>
	static T* buildObjectFromConstants(const string &name, const map<string, string> &object_const_map) {
		auto cptr = object_const_map.find(name);
		if ( cptr == object_const_map.end() ) {
			throw dnnException() << "Trying to build " << name << " from constants and can't find them\n";
		}

		istringstream *ss = new istringstream(cptr->second);
		Stream s(*ss, Stream::Text);
		T* n = s.readObject<T>();
		delete ss;
		return n;
	}

	void setInputModelStream(Stream *s) {
		input_stream = s;
	}


private:
	Stream *input_stream;
	const Constants &c;

};

}