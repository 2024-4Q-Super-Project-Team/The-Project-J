#pragma once
#include "Collider.h"

namespace Component
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* _owner);
		virtual ~BoxCollider() {}
	public:
		virtual void Start() override;
		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void PreRender() override;
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override;

	};
}

