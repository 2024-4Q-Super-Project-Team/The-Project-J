#pragma once
#include "Interface/ICycleHandler.h"

class ViewportScene;
class GameManager;
class World;

class WorldManager
	: public Engine::ICycleHandler
{

public:
	WorldManager(ViewportScene* _pViewport);
	~WorldManager();
public:
	void Start();
public:
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void EditorUpdate()	override;
	virtual void EditorRender()	override;
public:
	// 월드의 생성, 삭제, 전환을 해준다.
	void UpdateWorld();
public:
	World*	CreateWorld(const std::wstring& _name, std::wstring_view _tag = L"", bool isEmpty = false);
	BOOL	DestroyWorld(const std::wstring& _name);
	BOOL    SetActiveWorld(const std::wstring& _name);
	BOOL    SetActiveWorld(World* _pWorld);
	World*	FindWorld(const std::wstring _name);
public:
    auto						GetViewportScene() const { return mOwnerScene; }
	inline World*				GetActiveWorld() { return mCurrActiveWorld; }
	inline std::vector<World*>& GetWorlds() { return mWorldArray; }
private:
    ViewportScene* const    mOwnerScene;
	World*	                mCurrActiveWorld;
	World*	                mNextActiveWorld;
	std::vector<World*>     mWorldArray;
};