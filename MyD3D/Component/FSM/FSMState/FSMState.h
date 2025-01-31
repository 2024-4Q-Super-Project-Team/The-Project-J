#pragma once
#include "Component/MonoBehaviour/MonoBehaviour.h"

#define _FSM_STATE_INIT(Class) \
explicit Class(FiniteStateMachine* _fsm, Object* _owner) : FSMState(_fsm, _owner, #Class) {} \
virtual ~Class() = default;\

class FiniteStateMachine;

class FSMState : public MonoBehaviour
{
public:
	explicit FSMState(FiniteStateMachine* _fsm, Object* _owner, const std::string& _eid);
	virtual ~FSMState();
public:
	virtual void _CALLBACK OnStateEnter() {};
	virtual void _CALLBACK OnStateExit() {};
public:
	FiniteStateMachine* const FSM;
};