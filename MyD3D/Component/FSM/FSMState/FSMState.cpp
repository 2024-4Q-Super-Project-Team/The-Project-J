#include "pch.h"
#include "FSMState.h"

FSMState::FSMState(FiniteStateMachine* _fsm, Object* _owner, const std::string& _eid)
	: MonoBehaviour(_owner, _eid)
	, FSM(_fsm)
{
	mType = eComponentType::FSM_STATE;
	SetEID("FSMState : " + _eid);
}

FSMState::~FSMState()
{
}
