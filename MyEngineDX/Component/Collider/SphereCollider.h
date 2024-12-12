#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* _pOwner);
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
	virtual void Start() {}

	virtual bool Intersects(BoxCollider* box) override;
	virtual bool Intersects(SphereCollider* sphere) override;
	virtual bool Intersects(CapsuleCollider* capsule) override;
	virtual void Draw(GraphicsManager* _graphicsManager) override;

private:
	DirectX::BoundingSphere mSphere;

	friend class BoxCollider;
	friend class CapsuleCollider;
};

