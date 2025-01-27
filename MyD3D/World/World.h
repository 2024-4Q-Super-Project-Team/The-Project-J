#pragma once
#include "Interface/ICycleHandler.h"
#include "Interface/ICycleEventHandler.h"
#include "Helper/Entity/Entity.h"
#include "Interface/SaveBase.h"

class ViewportScene;
class Object;
class SkyBox;
class Camera;

class World
	: public Engine::ICycleHandler
	, public Engine::Entity
	, public Engine::SaveBase
	, public IEditorObject
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
	void EditorGlobalUpdate();
	virtual void EditorRender()	override;
public:
	virtual void _CALLBACK OnCreate()	override {};
	virtual void _CALLBACK OnDestroy()	override {};
	virtual void _CALLBACK OnEnable()	override;
	virtual void _CALLBACK OnDisable()	override;
public:
	void InitWorldObject();
    void UpdateObject();
public:
	// 오브젝트를 생성하는 함수
	Object* CreateObject(std::wstring_view _name, std::wstring_view _tag = L"");
	Object* FindObject(std::wstring_view _name);
	void	ShiftObject(Object* _dstObject);

	// 월드가 필요로하는 리소스를 미리 로드하기 위한 테이블
	std::unordered_set<std::wstring> mNeedResourceHandleTable;
private:
	bool				 isPersistance = false;
	ViewportScene* const mOwnerScene;
	std::vector<Object*> mObjectArray;
public:
	inline auto IsPersistance(bool _bIsPersistance) { return isPersistance = _bIsPersistance; }
	inline auto& IsPersistance() { return isPersistance; }
	inline auto& GetObjectArray() { return mObjectArray; }
	inline auto  GetOwnerViewportScene() const { return mOwnerScene; }
//////////////////////////////////////////////////////////////////////////////
// Json
//////////////////////////////////////////////////////////////////////////////
public:
	json Serialize();
	void Deserialize(json& j);
//////////////////////////////////////////////////////////////////////////////
// Physx
//////////////////////////////////////////////////////////////////////////////
public:
	// Rigidbody을 PxScene에 추가합니다. 
	void AddPxActor(PxActor* actor) { mPxScene->addActor(*actor); }
	void RemovePxActor(PxActor* actor) { mPxScene->removeActor(*actor); }
public:
	class PickingRay* mPickingRay;
	PxScene* mPxScene;
	class PhysicsEvent* mEventCallback;
public:
	PxScene* GetPxScene() { return mPxScene; }
	class PickingRay* GetPickingRay() { return mPickingRay; }
//////////////////////////////////////////////////////////////////////////////
// Editor
//////////////////////////////////////////////////////////////////////////////
public:
	bool isNodeOpen = false;
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

