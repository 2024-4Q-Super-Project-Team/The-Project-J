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
	// 매 틱마다 오브젝트 생성, 삭제 처리하는 함수
	void UpdateObject();
private:
	DXWorld* mOwnerWorld;
	std::list<GameObject*> mObjects;

	INT mGroupOrder;

	std::queue<GameObject*> mCreateQueue;
	std::queue<GameObject*> mDestroyQueue;
public:
	// 재할당 부담을 줄이려고 만든 오브젝트 벡터 Resize함수, 기존 사이즈보다 작으면 그냥 리턴
	inline void		SetListSize(UINT _size);

	// 그룹을 해당 월드로 이동. 해당 월드에 이미 같은 이름의 그룹이 있다면 합칠건지, 새로 
	inline void		SetWorld(DXWorld* _world);
	inline DXWorld* GetWorld() const { return mOwnerWorld; }

	// 레이어가 정렬되는 기준이 될 값.
	inline INT		GetOrder() const { return mGroupOrder; }
};

