#pragma once

#include "core.h"

namespace dnn {

template <typename Eval>
class EvalObject {
public:
	typedef Eval eval_type;
    virtual Eval eval() = 0;
};

template <typename Arg, typename Eval>
class EvalObjectDep : public EvalObject<Eval> {
public:
    EvalObjectDep(Arg _o)  : o(_o) {}
protected:
    Arg o;
};
template<typename State, typename Eval>
class DynamicObject : public EvalObject<Eval> {
public:
    virtual void step(State &dState_dt) = 0;
protected:
    State state;
};

template <typename Arg, typename State, typename Eval>
class DynamicObjectDep : public DynamicObject<State, Eval>, public EvalObjectDep<Arg, Eval> {
public:
    DynamicObjectDep(Arg _o) : EvalObjectDep<Arg, Eval>(_o) {}
};


template <typename Elem, typename Eval>
class System : public EvalObject<Eval> {
public:
	void add_elem(unique_ptr<Elem> e) {
		sys.push_back(std::move(e));
	}
protected:	
    vector< unique_ptr<Elem> > sys;
};

template <int StateSize, typename StateType = double>
class BaseState : private vector<StateType> {
public:	
    BaseState() : vector<StateType>(StateSize) {}
};


}