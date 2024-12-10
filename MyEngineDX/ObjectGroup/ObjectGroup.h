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
	~ObjectGroup();
public:
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
public:
	GameObject* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	const std::vector<GameObject*>& GetObjects() { return mObjects; }
	void SetListSize(UINT _size);
private:
	DXWorld* mOwnerWorld;
	std::vector<GameObject*> mObjects;

	std::queue<GameObject*> mCreateQueue;
	std::queue<GameObject*> mDestroyQueue;
public:
	void SetOwner(DXWorld* _ownerWorld);
	const DXWorld* GetOwner() { return mOwnerWorld; }
};

