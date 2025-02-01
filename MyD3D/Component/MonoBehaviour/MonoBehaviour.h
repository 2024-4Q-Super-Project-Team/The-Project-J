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

protected: // MonoBehaviour메소드==================
	// ===========================================
	// 오브젝트 검색===============================
	// 현재 월드를 기준으로 오브젝트의 이름과 태그가 둘다 같은 오브젝트를 반환
	Object* FindObject(std::wstring_view _name, std::wstring_view _tag);
	// 현재 월드를 기준으로 오브젝트 이름으로 검색하여 가장 처음에 나오는 오브젝트를 반환
	Object* FindObjectWithName(std::wstring_view _name);
	// 현재 월드를 기준으로 오브젝트를 태그로 검색하여 나온 오브젝트들을 배열로 반환
	std::vector<Object*> FindObjectsWithTag(std::wstring_view _tag);
	// 현재 월드를 기준으로 오브젝트를 이름으로 검색하여 나온 오브젝트들을 배열로 반환
	std::vector<Object*> FindObjectsWithName(std::wstring_view _name);
	// ===========================================
	// ===========================================
	// 오브젝트 생성 및 삭제========================
    // PrefabResource의 오브젝트를 복사합니다.
	Object* Instantiate(PrefabResource* _pInstant);
	Object* Instantiate(PrefabResource* _pInstant, Vector3 _position);
    Object* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	void    Destroy(Object* _object);

	//직렬화
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;

	void AddField(Serial* _serial)
	{
		mSerials.push_back(_serial);
	}
	// ===========================================
	// 
	// CallBack함수===============================

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