#pragma once
#include "Component/Component.h"

namespace Component
{
	class Rigidbody : public ComponentBase
	{
	public:
		Rigidbody(GameObject* _owner, bool _isDynamic);
		virtual ~Rigidbody() {}
	public:
		virtual void FixedUpdate() override;
		virtual void PreUpdate() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void PreRender() override;
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override;
		virtual void Start() {}

		PxRigidActor* GetRigidActor() { return mRigidActor; }

	protected:
		PxRigidActor* mRigidActor;
	};
}
