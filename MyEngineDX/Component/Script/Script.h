#pragma once
#include "Component/MonoBehaviour.h"
#include "Component/Component.h"

class GameObject;

namespace Component
{
	class Script
		: public Engine::MonoBehaviour
		, public ComponentBase
	{
	public:
		Script(GameObject* _owner);
		virtual ~Script() = default;
	public:
		virtual void FixedUpdate() override {}
		virtual void PreUpdate() override {}
		virtual void Update() override {}
		virtual void PostUpdate() override {}
		virtual void PreRender() override {}
		virtual void Render(GraphicsManager* _graphicsManager) override {}
		virtual void PostRender() override {}
	private:
	};
}