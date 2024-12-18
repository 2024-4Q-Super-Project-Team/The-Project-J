#pragma once
#include "Component/Component.h"

namespace Component
{
	class Animator : public ComponentBase
	{
	public:
		using ComponentBase::ComponentBase;
		virtual ~Animator();
	public:
		virtual void Tick() override;
		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void PreRender() override;
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override;
	private:

	};
}

