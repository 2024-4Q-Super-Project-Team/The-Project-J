#pragma once
#include "Component/Component.h"

class PrefabResource;
class Rigidbody;
class Object;
class MonoBehaviour;

#define _MONOBEHAVIOUR_INIT(Class) \
explicit Class(Object* _owner) : MonoBehaviour(_owner, #Class) {} \
virtual ~Class() = default;\

class MonoBehaviour
	: public Component
{
public:
    explicit MonoBehaviour(Object* _owner, const std::string& _eid);
    virtual ~MonoBehaviour() = default;
public:
    virtual void Start() override {}
    virtual void Tick() override {}
    virtual void FixedUpdate() override {}
    virtual void PreUpdate() override {}
    virtual void Update() override {}
    virtual void PostUpdate() override {}
    virtual void PreRender() override {}
    virtual void Render() override final {}
    virtual void Draw(Camera* _camera) override final {}
    virtual void PostRender() override {}
	// Editor Only
	virtual void EditorUpdate() override final {};
	virtual void EditorRender() override final {}

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
    // PrefabResource�� ������Ʈ�� �����մϴ�.
	Object* Instantiate(PrefabResource* _pInstant);
	Object* Instantiate(PrefabResource* _pInstant, Vector3 _position);
    Object* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	void    Destroy(Object* _object);

	//����ȭ
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;

	void AddField(Serial* _serial)
	{
		mSerials.push_back(_serial);
	}
	// ===========================================
	// 
	// CallBack�Լ�===============================

	virtual void _CALLBACK OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) {};
	virtual void _CALLBACK OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination) {};
	virtual void _CALLBACK OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) {};

	virtual void _CALLBACK OnTriggerEnter(Rigidbody* _origin, Rigidbody* _destination) {};
	virtual void _CALLBACK OnTriggerStay(Rigidbody* _origin, Rigidbody* _destination) {};
	virtual void _CALLBACK OnTriggerExit(Rigidbody* _origin, Rigidbody* _destination) {};

	virtual void _CALLBACK OnMouseEnter() {};
	virtual void _CALLBACK OnMouseStay() {};
	virtual void _CALLBACK OnMouseExit() {};

	virtual void _CALLBACK OnAnimationPlay() {};
	virtual void _CALLBACK OnAnimationStop() {};
	virtual void _CALLBACK OnAnimationStart() {};
	virtual void _CALLBACK OnAnimationEnd() {};

	std::vector<Serial*> mSerials;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;

};