#pragma once
#include "Interface/ICycleHandler.h"

class ViewportScene;
class GameManager;
class World;

class WorldManager
	: public Engine::ICycleHandler
{
    using WorldHashMap = std::unordered_map<std::wstring, World*>;
public:
	WorldManager(ViewportScene* _pViewport);
	~WorldManager();
public:
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
public:
	// 월드의 생성, 삭제, 전환을 해준다.
	void UpdateWorld();
public:
	template<class T>
	World*	CreateWorld(const std::wstring& _name, std::wstring_view _tag = L"", bool isEmpty = false);
	World*	CreateEmptyWorld(const std::wstring& _name, std::wstring_view _tag = L"");
	World*	GetActiveWorld();
	BOOL	DestroyWorld(const std::wstring& _name);
	BOOL    SetActiveWorld(const std::wstring& _name);
	BOOL    SetActiveWorld(World* _pWorld);

	void SaveWorlds();
	void LoadWorlds();
public:
    auto    GetViewportScene() const { return mOwnerScene; }
private:
    ViewportScene* const    mOwnerScene;
	World*	                mCurrActiveWorld;
	World*	                mNextActiveWorld;
    WorldHashMap            mWorlds;
};

template<class T>
World* WorldManager::CreateWorld(const std::wstring& _name, std::wstring_view _tag, bool isEmpty)
{
    auto itr = Helper::FindMap(_name, mWorlds);
    T* instance;
    if (itr != nullptr)
    {
        instance = dynamic_cast<T*>(*itr);
        if (instance)
            return instance;
        else
            return nullptr;
    }  
    else
    {
        instance = new T(mOwnerScene, _name, _tag, isEmpty);
        mWorlds[_name] = instance;
        instance->OnCreate();
        return instance;
    }
}
