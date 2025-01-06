#pragma once
#include "Interface/ICycleHandler.h"
#include "Helper/Entity/Entity.h"

class World;
class Object;
class Camera;

class ObjectGroup
	: public Engine::ICycleHandler
	, public Engine::Entity
{
public:
    explicit ObjectGroup(std::wstring_view _name, std::wstring_view _tag);
	virtual ~ObjectGroup();
public:
	virtual void Tick()			override;
	virtual void FixedUpdate()	override;
	virtual void PreUpdate()	override;
	virtual void Update()		override;
	virtual void PostUpdate()	override;
	virtual void PreRender()	override;
	virtual void Render()       override;
	virtual void Draw(Camera* _camera);
	virtual void PostRender()	override;
private:
    // 매 틱마다 오브젝트 생성, 삭제 처리하는 함수
    void            UpdateObject();
public:
    // 오브젝트를 생성하는 함수
    Object*         CreateObject(std::wstring_view _name, std::wstring_view _tag);
	// 재할당 부담을 줄이려고 만든 오브젝트 벡터 Resize함수, 기존 사이즈보다 작으면 그냥 리턴
	void		    SetListSize(UINT _size);
	// 그룹을 해당 월드로 이동. 해당 월드에 이미 같은 이름의 그룹이 있다면 합칠건지, 새로 
	void		    SetWorld(World* _world);
    // 오브젝트 컨테이너를 반환
    inline auto&    GetObjects() const { return mObjects; }
	inline World*	GetWorld() const { return mOwnerWorld; }
	inline INT		GetOrder() const { return mGroupOrder; }
private:
	World* mOwnerWorld;
	std::list<Object*> mObjects;
	INT mGroupOrder;
};

