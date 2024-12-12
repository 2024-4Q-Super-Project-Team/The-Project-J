#pragma once
#include "Collider.h"

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

enum COLLISION_LAYER
{
	NONE,
	WALL,
	OBJECT,
	PLAYER
};

class ColliderManager : public Engine::IEngineCycle
{
public:
	virtual void FixedUpdate() {};
	virtual void PreUpdate() {};
	virtual void Update();
	virtual void PostUpdate() {};
	virtual void PreRender() {};
	virtual void Render(GraphicsManager* _graphicsManager) {};
	virtual void PostRender() {};

private:
	void CheckCollision(); 
	bool TestCollisionByType(Collider* _c1, Collider* _c2);
	bool CheckLayer(Collider* _c1, Collider* _c2);
	void CallWhenCollisionFuncions(Collider* _c1, Collider* _c2);
	void CallOnCollisionExitFuncion(Collider* _c1, Collider* _c2);

	bool CheckCollisionBoxToBox(BoxCollider* box1, BoxCollider* box2);							//Box-Box
	bool CheckCollisionBoxToSphere(BoxCollider* box, SphereCollider* sphere);					//Box-Sphere
	bool CheckCollisionBoxToCapsule(BoxCollider* box, CapsuleCollider* capsule);				//Box-Capsule
	bool CheckCollisionSphereToSphere(SphereCollider* sphere1, SphereCollider* sphere2);		//Sphere-Sphere
	bool CheckCollisionSphereToCapsule(SphereCollider* sphere, CapsuleCollider* capsule);		//Sphere-Capsule
	bool CheckCollisionCapsuleToCapsule(CapsuleCollider* capsule1, CapsuleCollider* capsule2);	//Capsule-Capsule


private:
	std::vector<Collider*> mColliders;
};

