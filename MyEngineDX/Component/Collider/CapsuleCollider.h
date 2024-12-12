#pragma once
#include "Collider.h"

struct Cylinder
{
	Vector3 startPoint;
	Vector3 endPoint;
	float radius;
};


class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(GameObject* _pOwner);
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
	Vector3 FindClosestPointOnSegment(Vector3 point, Vector3 start, Vector3 end);

private:
	Cylinder mCylinder;
	DirectX::BoundingSphere mSpheres[2];
};

