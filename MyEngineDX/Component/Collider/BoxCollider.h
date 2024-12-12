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

	virtual bool Intersects(BoxCollider* box);
	virtual bool Intersects(SphereCollider* sphere);
	virtual bool Intersects(CapsuleCollider* capsule);

private:
	void GetAxis(Vector3& axisX, Vector3& axisY, Vector3& axisZ);

	DirectX::BoundingOrientedBox mBox;

	friend class SphereCollider;
	friend class CapsuleCollider;
};

