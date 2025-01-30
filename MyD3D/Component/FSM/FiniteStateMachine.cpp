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
}

void FiniteStateMachine::FixedUpdate()
{
}

void FiniteStateMachine::PreUpdate()
{
}

void FiniteStateMachine::Update()
{
}

void FiniteStateMachine::PostUpdate()
{
}

void FiniteStateMachine::PreRender()
{
}

void FiniteStateMachine::Render()
{
}

void FiniteStateMachine::Draw(Camera* _camera)
{
}

void FiniteStateMachine::PostRender()
{
}

void FiniteStateMachine::EditorUpdate()
{
}

void FiniteStateMachine::EditorRender()
{
}

void FiniteStateMachine::UpdateState()
{
}
