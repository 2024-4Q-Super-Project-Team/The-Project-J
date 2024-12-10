#pragma once
#include "ObjectGroup/ObjectGroup.h"

class WorldManager;

class DXWorld 
	: public Engine::IEngineCycle
	, public Engine::Entity
{
public:
	DXWorld(WorldManager* _wrdMng, std::wstring_view _name, std::wstring_view _tag);
	virtual ~DXWorld();
public:
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

	virtual void _CALLBACK OnFixedUpdate() {};
	virtual void _CALLBACK OnPreUpdate() {};
	virtual void _CALLBACK OnUpdate() {};
	virtual void _CALLBACK OnPostUpdate() {};
	virtual void _CALLBACK OnPreRender() {};
	virtual void _CALLBACK OnRender() {};
	virtual void _CALLBACK OnPostRender() {};
public:
	ObjectGroup* CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag);
	ObjectGroup* GetObjectGroup(std::wstring_view _name);
private:
	Transform3D* mWorldTransform;
	std::unordered_map<std::wstring, ObjectGroup*> mObjectGroups;
	static std::unordered_map<std::wstring, ObjectGroup*> mPersistanceObjectGroups;

	std::queue<ObjectGroup*> mCreateQueue;
	std::queue<ObjectGroup*> mDestroyQueue;
protected:
	WorldManager* const mWorldManager;

	friend class WorldManager;
};