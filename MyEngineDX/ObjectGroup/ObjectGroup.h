#pragma once
#include "Object/Object.h"

namespace Graphics
{
	class GraphicsDevice;
	class Renderer;
	class RenderTarget;
}
class DXWorld;

class ObjectGroup
	: public Engine::IEngineCycle
	, public Engine::Entity
{
public:
	ObjectGroup();
	ObjectGroup(std::wstring_view _name, std::wstring_view _tag);
	virtual ~ObjectGroup();
public:
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
public:
	GameObject* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	const std::list<GameObject*>& GetObjects() { return mObjects; }
private:
	// �� ƽ���� ������Ʈ ����, ���� ó���ϴ� �Լ�
	void UpdateObject();
private:
	DXWorld* mOwnerWorld;
	std::list<GameObject*> mObjects;

	INT mGroupOrder;

	std::queue<GameObject*> mCreateQueue;
	std::queue<GameObject*> mDestroyQueue;
public:
	// ���Ҵ� �δ��� ���̷��� ���� ������Ʈ ���� Resize�Լ�, ���� ������� ������ �׳� ����
	inline void		SetListSize(UINT _size);

	// �׷��� �ش� ����� �̵�. �ش� ���忡 �̹� ���� �̸��� �׷��� �ִٸ� ��ĥ����, ���� 
	inline void		SetWorld(DXWorld* _world);
	inline DXWorld* GetWorld() const { return mOwnerWorld; }

	// ���̾ ���ĵǴ� ������ �� ��.
	inline INT		GetOrder() const { return mGroupOrder; }
};

