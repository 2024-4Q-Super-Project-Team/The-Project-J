#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* _pOwner) : Collider(_pOwner, COLLIDER_TYPE_SPHERE) {}
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
	virtual void Start() {}

private:
	DirectX::BoundingSphere obb;
};

