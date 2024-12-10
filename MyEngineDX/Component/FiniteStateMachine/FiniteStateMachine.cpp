#include "pch.h"
#include "FiniteStateMachine.h"

namespace Component
{
	FiniteStateMachine::FiniteStateMachine(Object* _owner) : Component(_owner)
	{
		mType = ComponentType::FiniteStateMachine;
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

	void FiniteStateMachine::LateUpdate()
	{
	}
	void FiniteStateMachine::PreRender()
	{
	}
	void FiniteStateMachine::Render(Graphics::IRenderer* _renderer)
	{
	}
	void FiniteStateMachine::PostRender()
	{
	}
}