#pragma once
#include "Component/MonoBehaviour.h"
#include "Component/Component.h"

class Object;

namespace Component
{
	class FiniteStateMachine
		: public Engine::MonoBehaviour
		, public ComponentBase
	{
	public:
		FiniteStateMachine(Object* _owner);
		virtual ~FiniteStateMachine();

		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void PreRender() override;
		virtual void Render(Graphics::IRenderer* _renderer) override;
		virtual void PostRender() override;
	private:
	};
}