#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* _pOwner) : Collider(_pOwner, COLLIDER_TYPE_BOX) {}
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
	virtual void Start() {}

private:
	DirectX::BoundingOrientedBox obb;
};

