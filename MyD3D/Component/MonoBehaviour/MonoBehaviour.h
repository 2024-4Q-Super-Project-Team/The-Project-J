#pragma once
#include "Component/Component.h"

class Prefab;
class Collider;
class Object;

class MonoBehaviour
	: public Component
{
public:
    explicit MonoBehaviour(Object* _owner);
    virtual ~MonoBehaviour() = default;
public:
    virtual void Start() override {};
    virtual void Tick() override {};
    virtual void FixedUpdate() override {};
    virtual void PreUpdate() override {};
    virtual void Update() override {};
    virtual void PostUpdate() override {};
    virtual void PreRender() override {};
    virtual void Render() override {};
    virtual void Draw(Camera* _camera) override {};
    virtual void PostRender() override {};
protected: // MonoBehaviour�޼ҵ�==================
	// ===========================================
	// ������Ʈ �˻�===============================
	// ���� ���带 �������� ������Ʈ�� �̸��� �±װ� �Ѵ� ���� ������Ʈ�� ��ȯ
	Object* FindObject(std::wstring_view _name, std::wstring_view _tag);
	// ���� ���带 �������� ������Ʈ �̸����� �˻��Ͽ� ���� ó���� ������ ������Ʈ�� ��ȯ
	Object* FindObjectWithName(std::wstring_view _name);
	// ���� ���带 �������� ������Ʈ�� �±׷� �˻��Ͽ� ���� ������Ʈ���� �迭�� ��ȯ
	std::vector<Object*> FindObjectsWithTag(std::wstring_view _tag);
	// ���� ���带 �������� ������Ʈ�� �̸����� �˻��Ͽ� ���� ������Ʈ���� �迭�� ��ȯ
	std::vector<Object*> FindObjectsWithName(std::wstring_view _name);
	// ===========================================
	// ===========================================
	// ������Ʈ ���� �� ����========================
    // Prefab�� ������Ʈ�� �����մϴ�.
	Object* Instantiate(Prefab* _pInstant);
	Object* Instantiate(Prefab* _pInstant, Vector3 _position);
    Object* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	void    Destroy(Object* _object);
	// ===========================================
	// 
	// CallBack�Լ�===============================
public:
	virtual void _CALLBACK OnEnable() {};
	virtual void _CALLBACK OnDisable() {};
	virtual void _CALLBACK OnDestroy() {};

	virtual void _CALLBACK OnCollisionEnter(Collider* _origin, Collider* _destination) {};
	virtual void _CALLBACK OnCollisionStay(Collider* _origin, Collider* _destination) {};
	virtual void _CALLBACK OnCollisionExit(Collider* _origin, Collider* _destination) {};

	virtual void _CALLBACK OnMouseEnter() {};
	virtual void _CALLBACK OnMouseStay() {};
	virtual void _CALLBACK OnMouseExit() {};

	virtual void _CALLBACK OnAnimationPlay() {};
	virtual void _CALLBACK OnAnimationStop() {};
	virtual void _CALLBACK OnAnimationStart() {};
	virtual void _CALLBACK OnAnimationEnd() {};
protected:

};
