#pragma once
#include "Interface/ICycleHandler.h"
#include "Interface/SaveBase.h"
#include "Editor/Interface/IEditorObject.h"

class World;
class Object;
class Camera;

namespace Editor
{
	class Widget;
	class InputVector3;
}

struct Serial;
template <typename T>
struct SerialData;


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
	RIGIDBODY,
	COLLDIER,
	CONTROLLER,
	UPDATE_END,  // 업데이트 루프를 도는 컴포넌트의 끝. 빈 배열 하나를 차지하더라도 있는게 나은듯
    MESH_FILTER,
    SIZE,
};

constexpr static UINT ComponentSize = static_cast<UINT>(eComponentType::SIZE);

class Component
	: public Engine::ICycleHandler
	, public Engine::SaveBase
	, public IEditorObject
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
    // 실질적인 DrawCall을 하는 곳
	virtual void Render() = 0;
    // Draw할 메쉬들을 커맨드로 모으는 과정
    virtual void Draw(Camera* _camera) = 0;
	virtual void PostRender() = 0;

public:
	inline eComponentType	GetType()				{ return mType; }
	inline void				SetActive(bool _active) { isActive = _active;}
	inline bool				IsActive()				{ return isActive; }
	bool					IsAwake()				{ return mIsAwake; }
	void					Wake()					{ mIsAwake = true; }
	void					UnWake()					{ mIsAwake = false; }
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) {};



	virtual json Serialize() = 0;
	virtual void Deserialize(json& j) = 0;
public:
	Object* const	gameObject;
protected:
	eComponentType	mType;
	bool			isActive; 
private:
	bool			mIsAwake = false; //깨어났는지를 나타냅니다. Start함수 호출 여부와 같습니다. 
public:
	virtual void EditorRendering() override;
};

class DynamicComponent
    : public Component
{

};
