#pragma once

class GameManager;
class WorldManager;
class ObjectGroup;

class DXWorld 
	: public Engine::IEngineCycle
	, public Engine::Entity
{
public:
	DXWorld(std::wstring_view _name, std::wstring_view _tag);
	virtual ~DXWorld();
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
	virtual void _CALLBACK OnEnable() override {};
	virtual void _CALLBACK OnDisable() override {};
	virtual void _CALLBACK OnCreate() override {};
	virtual void _CALLBACK OnDestroy() override {};
	virtual void _CALLBACK OnLoad() {};
	virtual void _CALLBACK OnUnLoad() {};

	virtual void _CALLBACK OnTick() {};
	virtual void _CALLBACK OnFixedUpdate() {};
	virtual void _CALLBACK OnPreUpdate() {};
	virtual void _CALLBACK OnUpdate() {};
	virtual void _CALLBACK OnPostUpdate() {};
	virtual void _CALLBACK OnPreRender() {};
	virtual void _CALLBACK OnRender() {};
	virtual void _CALLBACK OnPostRender() {};
public:
	// 오브젝트 그룹을 만듭니다.
	ObjectGroup* CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag = L"");
	// 오브젝트 그룹을 호출한 월드로 옮깁니다. 속한 월드가 같으면 그냥 리턴
	void		 CreateObjectGroup(ObjectGroup* _recvGroup);
	// 오브젝트 그룹을 이름으로 검색합니다. 없을 시 nullptr 반환
	ObjectGroup* GetObjectGroup(std::wstring_view _name);
	const std::vector<ObjectGroup*>& GetObjectGroups() { return mObjectGroups; }
private:
	void UpdateGroup();
public:
	PxScene*		GetScene() { return mScene; }
private:
	Transform* mWorldTransform;
	std::vector<ObjectGroup*> mObjectGroups;
protected:
	PxScene* mScene;
};