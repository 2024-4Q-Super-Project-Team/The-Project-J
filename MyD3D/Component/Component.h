#pragma once
#include "Interface/ICycleHandler.h"

#include <nlohmann/json.hpp>
using namespace nlohmann;

class World;
class Object;
class Camera;

enum class eComponentType
{
	Transform,
	SCRIPT,
	FINITE_STATE_MACHINE,
	CAMERA,
	LIGHT,
	ANIMAITOR,
	SKINNED_MESH_RENDERER,
	MESH_RENDERER,
    EDITOR_UI,
	UPDATE_END,  // ������Ʈ ������ ���� ������Ʈ�� ��. �� �迭 �ϳ��� �����ϴ��� �ִ°� ������
    MESH_FILTER,
    SIZE,
};

constexpr static UINT ComponentSize = static_cast<UINT>(eComponentType::SIZE);

class Component
	: public Engine::ICycleHandler
{
public:
	explicit Component(Object* _owner);
	virtual ~Component();
public:
    virtual void Start() = 0;
	virtual void Tick()	= 0;
	virtual void FixedUpdate() = 0;
	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void PreRender() = 0;
    // �������� DrawCall�� �ϴ� ��
	virtual void Render() = 0;
    // Draw�� �޽����� Ŀ�ǵ�� ������ ����
    virtual void Draw(Camera* _camera) = 0;
	virtual void PostRender() = 0;
	//����ȭ �� ������ȭ 
	virtual json Serialize() = 0;
	virtual json Deserialize() = 0;
public:
	inline eComponentType	GetType()				{ return mType; }
	inline void				SetActive(bool _active) { isActive = _active; }
	inline bool				IsActive()				{ return isActive; }
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) {};
public:
	Object* const	gameObject;
protected:
	eComponentType	mType;
	bool			isActive;
public:
    virtual void EditorRendering() {};
};

class DynamicComponent
    : public Component
{

};