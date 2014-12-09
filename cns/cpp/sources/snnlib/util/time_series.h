#pragma once

#include <snnlib/base.h>
#include <snnlib/protos/time_series.pb.h>
#include <snnlib/serialize/serialize.h>
#include <snnlib/serialize/proto_rw.h>

class TimeSeries : public Printable {
public:
    vector<double> data;

    TimeSeries() {}

    TimeSeries(const vector<double> &_data) : data(_data) {}

    void deserialize(const Protos::TimeSeries &m) {
        for(size_t i=0; i<m.data_size(); i++) {
            data.push_back( m.data(i) );
        }
    }
    void print(std::ostream& str) const {
        print_vector<double>(data, str, ", ");
        str << "\n";
    }
    void push_back(const double &x) {
        data.push_back(x);
    }
    size_t size() {
        return data.size();
    }
};


class LabeledTimeSeries  :  public Serializable<Protos::LabeledTimeSeries> {
public:
    string label;
    TimeSeries ts;

    LabeledTimeSeries() : Serializable(ELabeledTimeSeries) {}
    LabeledTimeSeries(const vector<double> &data, string _label) : Serializable(ELabeledTimeSeries), ts(data), label(_label) { }
    LabeledTimeSeries(const Protos::LabeledTimeSeries &m) : Serializable(ELabeledTimeSeries) {
        label = m.label();
        ts.deserialize(m.ts());
    }


    void print(std::ostream& str) const {
        str << "label: " << label << "\n";
        str << "data: " << ts;
    }

    // Serializable:
    ProtoPack serialize() {
        Protos::LabeledTimeSeries *stat = getNewMessage();
        Protos::TimeSeries *new_ts = stat->mutable_ts();
        for(auto it=ts.data.begin(); it != ts.data.end(); ++it) {
            new_ts->add_data(*it);
        }
        stat->set_label(label);
        return ProtoPack({stat});
    }
    void deserialize() {
        Protos::LabeledTimeSeries * m = getSerializedMessage();
        label = m->label();
        ts.deserialize(m->ts());
    }
    // end Serializable

};



class LabeledTimeSeriesList  : public Serializable<Protos::LabeledTimeSeriesList> {
public:
    LabeledTimeSeriesList() : Serializable(ELabeledTimeSeriesList) {
    }

    // Serializable:
    ProtoPack serialize() {
        Protos::LabeledTimeSeriesList *l = getNewMessage();
        for(auto it=ts.begin(); it != ts.end(); ++it) {
            Protos::LabeledTimeSeries *lts = l->add_list();
            Protos::LabeledTimeSeries *lts_serial = castProtoMessage<Protos::LabeledTimeSeries>(it->serialize().front());
            *lts = *lts_serial;
        }
        return ProtoPack({l});
    }

    void deserialize() {
        Protos::LabeledTimeSeriesList *m = getSerializedMessage();
        for(size_t li=0; li < m->list_size(); li++) {
            push_back(m->list(li));
        }
    }
    // end Serializable

    void push_back(LabeledTimeSeries lst) {
        ts.push_back(lst);
    }

    void print(std::ostream& str) const {
        print_vector<LabeledTimeSeries>(ts, str, "\n");
    }
    size_t size() const {
        return ts.size();
    }

    vector<LabeledTimeSeries> ts;
};


class ContLabeledTimeSeries : public Printable {
public:
    ContLabeledTimeSeries() {}
    ContLabeledTimeSeries(LabeledTimeSeriesList lst, const double &dt) {
        init(lst, dt);
    }

    void init(LabeledTimeSeriesList lst, const double &dt) {
        double acc_time = 0;
        auto engine = std::default_random_engine{};
        std::random_shuffle(lst.ts.begin(), lst.ts.end());

        for(auto it=lst.ts.begin(); it != lst.ts.end(); ++it) {
            for(auto it_val=it->ts.data.begin(); it_val != it->ts.data.end(); ++it_val) {
                data.push_back(*it_val);
            }

            int pos = -1;
            if(labels.size()>0) {
                auto pos_it = find(labels.begin(), labels.end(), it->label);
                if(pos_it != labels.end()) {
                    pos = pos_it - labels.begin();
                }
            }

            if(pos<0) {
                labels.push_back(it->label);
                pos = labels.size()-1;
            }

            labels_id_timeline.push_back(pos);
            acc_time += dt*(double)it->ts.size();
            timeline.push_back(acc_time);
         }

         if(timeline.size()>0) {
            Tmax = timeline.back();
         }
         current_position = 0;
    }

    const size_t* getCurrentClassId(const double &t) {
        while(current_position < timeline.size()) {
            if(t <= timeline[current_position]) {
                return &labels_id_timeline[current_position];
            }
            current_position += 1;
        }
        cerr << "Trying to get current class for time bigger than Tmax\n";
        terminate();
    }

    inline void pop_value() {
        data.pop_front();
    }
    inline const double& top_value() {
        return *data.begin();
    }
    inline size_t size() const {
        return data.size();
    }
    void print(std::ostream& str) const {
        str << "data: "; print_deque<double>(data, str,","); str << "\n";
        str << "labels: "; print_vector<string>(labels, str,","); str << "\n";
        str << "timeline: "; print_vector<double>(timeline,str,","); str << "\n";
        str << "labels id timeline: "; print_vector<size_t>(labels_id_timeline,str,","); str << "\n";
        str << "Tmax: " << Tmax << "\n";
    }
    void reset() {
        current_position = 0;
    }

    size_t current_position;
    double Tmax;

    deque<double> data;
    vector<string> labels;

    vector<size_t> labels_id_timeline;
    vector<double> timeline;
};

