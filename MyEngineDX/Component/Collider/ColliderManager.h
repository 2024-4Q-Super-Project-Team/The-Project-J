#pragma once
#include "Collider.h"

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

const int LAYER_MAX_SIZE = 20;

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

public:
	void SetLayerTable(int _layer1, int _layer2, bool _bCollide);

	//�ݶ��̴��� �߰��մϴ�.(�ݶ��̴� ���� �� ����)
	void AddCollider(Collider* _collider) { mColliders.push_back(_collider); }

	//������Ʈ�� �ݶ��̴��� ��� �����մϴ�.(������Ʈ ���� �� ����)
	void RemoveColliders(GameObject* _object);

private:
	void CheckCollision(); 
	bool TestCollisionByType(Collider* _c1, Collider* _c2);
	bool CheckLayer(Collider* _c1, Collider* _c2);
	void CallWhenCollisionFuncions(Collider* _c1, Collider* _c2);
	void CallOnCollisionExitFuncion(Collider* _c1, Collider* _c2);

private:
	std::vector<Collider*> mColliders;

	std::array<std::array<bool, LAYER_MAX_SIZE>, LAYER_MAX_SIZE> mLayerTable{};
};

