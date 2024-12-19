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
	// ������Ʈ �׷��� ����ϴ�.
	ObjectGroup* CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag = L"");
	// ������Ʈ �׷��� ȣ���� ����� �ű�ϴ�. ���� ���尡 ������ �׳� ����
	void		 CreateObjectGroup(ObjectGroup* _recvGroup);
	// ������Ʈ �׷��� �̸����� �˻��մϴ�. ���� �� nullptr ��ȯ
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