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
    // �� ƽ���� ������Ʈ ����, ���� ó���ϴ� �Լ�
    void            UpdateObject();
public:
    // ������Ʈ�� �����ϴ� �Լ�
    Object*         CreateObject(std::wstring_view _name, std::wstring_view _tag);
	// ���Ҵ� �δ��� ���̷��� ���� ������Ʈ ���� Resize�Լ�, ���� ������� ������ �׳� ����
	void		    SetListSize(UINT _size);
	// �׷��� �ش� ����� �̵�. �ش� ���忡 �̹� ���� �̸��� �׷��� �ִٸ� ��ĥ����, ���� 
	void		    SetWorld(World* _world);
    // ������Ʈ �����̳ʸ� ��ȯ
    inline auto&    GetObjects() const { return mObjects; }
	inline World*	GetWorld() const { return mOwnerWorld; }
	inline INT		GetOrder() const { return mGroupOrder; }
private:
	World* mOwnerWorld;
	std::list<Object*> mObjects;
	INT mGroupOrder;
};

