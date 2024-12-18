#pragma once
#include "World/World.h"

class GameManager;

class WorldManager
	: public Engine::IEngineCycle
{
public:
	WorldManager();
	~WorldManager();
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
	template<class T>
	DXWorld* CreateWorld(std::wstring_view _name, std::wstring_view _tag);
	DXWorld* GetActiveWorld();
	BOOL	 SetActiveWorld(std::wstring_view _name);
private:
	DXWorld* mCurrActiveWorld;
	DXWorld* mNextActiveWorld;
	std::unordered_map<std::wstring, DXWorld*> mWorlds;
private:
	std::queue<DXWorld*> mCreateQueue;
	std::queue<DXWorld*> mDestroyQueue;
};

template<class T>
inline DXWorld* WorldManager::CreateWorld(std::wstring_view _name, std::wstring_view _tag)
{
	auto itr = mWorlds.find(_name.data());
	if (itr != mWorlds.end())
		return (*itr).second;
	T* instance = new T(_name, _tag);
	mCreateQueue.push(instance);
	instance->OnCreate();

	if (nullptr == mCurrActiveWorld)
	{
		mCurrActiveWorld = instance;
		mCurrActiveWorld->OnLoad();
	}
	return instance;
}
