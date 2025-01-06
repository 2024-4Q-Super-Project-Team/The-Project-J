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
    // Prefab의 오브젝트를 복사합니다.
	Object* Instantiate(Prefab* _pInstant);
	Object* Instantiate(Prefab* _pInstant, Vector3 _position);
    Object* CreateObject(std::wstring_view _name, std::wstring_view _tag);
	void    Destroy(Object* _object);
	// ===========================================
	// 
	// CallBack함수===============================
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
