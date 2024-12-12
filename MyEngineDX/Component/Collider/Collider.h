#pragma once
#include "Component/Component.h"

enum COLLIDER_TYPE
{
	COLLIDER_TYPE_BOX,
	COLLIDER_TYPE_SPHERE,
	COLLIDER_TYPE_CAPSULE
};

class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class GraphicsManager;

class Collider : public Component::ComponentBase
{
public:
	Collider(GameObject* _pOwner, COLLIDER_TYPE _type) : ComponentBase(_pOwner), mType(_type) {}
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
	virtual void Start() {}

	virtual bool Intersects(BoxCollider* box) abstract;
	virtual bool Intersects(SphereCollider* sphere) abstract;
	virtual bool Intersects(CapsuleCollider* capsule) abstract;
	virtual void Draw(GraphicsManager* _graphicsManager) abstract;

	void SetLayer(int layer) { mLayer = layer; }
	int GetLayer() { return mLayer; }
	COLLIDER_TYPE GetType() { return mType; }
	bool GetIsColliding() { return mIsColliding; }
	void SetIsColliding(bool isColliding) { mIsColliding = isColliding; }
	void SetDebugMode(bool mode) { mDebugMode = mode; }
private:
	COLLIDER_TYPE mType; 
	int mLayer = 0;
	bool mIsColliding = false;
	bool mDebugMode = false;

	friend class CollisionManager;
};

