#include "pch.h"
#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(Object* _owner)
	: Component(_owner)
{
	SetEID("FiniteStateMachine");
	mType = eComponentType::FINITE_STATE_MACHINE;
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::Start()
{
}

void FiniteStateMachine::Tick()
{
	UpdateState();
	if (mCurrState)
		mCurrState->Tick();
}

void FiniteStateMachine::FixedUpdate()
{
	if (mCurrState)
		mCurrState->FixedUpdate();
}

void FiniteStateMachine::PreUpdate()
{
	if (mCurrState)
		mCurrState->PreUpdate();
}

void FiniteStateMachine::Update()
{
	if (mCurrState)
		mCurrState->Update();
}

void FiniteStateMachine::PostUpdate()
{
	if (mCurrState)
		mCurrState->PostUpdate();
}

void FiniteStateMachine::PreRender()
{
	if (mCurrState)
		mCurrState->PreRender();
}

void FiniteStateMachine::Render()
{
}

void FiniteStateMachine::Draw(Camera* _camera)
{
}

void FiniteStateMachine::PostRender()
{
	if (mCurrState)
		mCurrState->PostRender();
}

void FiniteStateMachine::EditorUpdate()
{
	UpdateState();
	if (mCurrState)
		mCurrState->EditorUpdate();
}

void FiniteStateMachine::EditorRender()
{
	if (mCurrState)
		mCurrState->EditorRender();
}

void FiniteStateMachine::UpdateState()
{
	// 다음 상태가 현재 상태와 다르다면
	if (mCurrState != mNextState)
	{
		// 다르면 현재상태의 Exit, 다음 상태의 Enter를 호출
		mCurrState->OnStateExit();
		mNextState->OnStateEnter();
		mCurrState = mNextState;
	}
}

void FiniteStateMachine::ChangeState(const std::wstring& _key)
{
	FSMState* state = GetState<FSMState>(_key);
	if (state && state != mNextState)
	{
		mNextState = state;
	}
}

void FiniteStateMachine::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

}

json FiniteStateMachine::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "FiniteStateMachine";
	return ret;
}

void FiniteStateMachine::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());
}
