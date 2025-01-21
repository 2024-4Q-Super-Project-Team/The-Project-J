#pragma once
#include "Interface/ICycleHandler.h"
#include "Interface/ICycleEventHandler.h"
#include "Helper/Entity/Entity.h"
#include "Interface/SaveBase.h"

class LightSystem;
class ViewportScene;
class ObjectGroup;
class SkyBox;
class Camera;

class World
	: public Engine::ICycleHandler
	, public Engine::ICycleEventHandler
	, public Engine::Entity
	, public Engine::SaveBase
{
public:
	explicit World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty = false);
	virtual ~World();
public:
	void Start();
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

	virtual void EditorUpdate()	override;
	virtual void EditorRender()	override;
public:
	virtual void _CALLBACK OnCreate()		override {};
	virtual void _CALLBACK OnDestroy()		override {};
	virtual void _CALLBACK OnEnable()		override {};
	virtual void _CALLBACK OnDisable()		override {};
	virtual void _CALLBACK OnTick()			override {};
	virtual void _CALLBACK OnFixedUpdate()	override {};
	virtual void _CALLBACK OnPreUpdate()	override {};
	virtual void _CALLBACK OnUpdate()		override {};
	virtual void _CALLBACK OnPostUpdate()	override {};
	virtual void _CALLBACK OnPreRender()	override {};
	virtual void _CALLBACK OnRender()		override {};
	virtual void _CALLBACK OnPostRender()	override {};
public:
	void InitWorldObject();
    void UpdateGroup();
public:
    // 오브젝트 그룹을 만듭니다.
    ObjectGroup*    CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag = L"");
	// Rigidbody을 PxScene에 추가합니다. 
	void AddPxActor(PxActor* actor) { mPxScene->addActor(*actor); }
    // 오브젝트 그룹을 호출한 월드로 옮깁니다. 속한 월드가 같으면 그냥 리턴
    void		    ReceiveObjectGroup(ObjectGroup* _recvGroup);
    // 오브젝트 그룹을 이름으로 검색합니다. 없을 시 nullptr 반환
    ObjectGroup*    GetObjectGroup(std::wstring_view _name);
    // 오브젝트 그룹 컨테이너를 반환
    inline const std::vector<ObjectGroup*>& GetObjectGroups() { return mObjectGroups; }
	//직렬화
	json Serialize();
	void Deserialize(json& j);
public:
    inline auto GetOwnerViewportScene() const { return mOwnerScene; }
    inline auto GetLightSystem() const { return mLightSystem; }
	PxScene* GetPxScene() { return mPxScene; }
	class PickingRay* GetPickingRay() { return mPickingRay; }
protected:
    ViewportScene* const mOwnerScene;
private:
    std::vector<ObjectGroup*> mObjectGroups;
    LightSystem* mLightSystem;
	class PickingRay* mPickingRay;

	PxScene* mPxScene;
	class PhysicsEvent* mEventCallback;
};

