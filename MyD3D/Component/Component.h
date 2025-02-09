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
	FINITE_STATE_MACHINE,
	CAMERA,
	LIGHT,
	ANIMAITOR,
	BONE_ATTACHER,
	SKINNED_MESH_RENDERER,
	MESH_RENDERER,
	EDITOR_UI,
	RIGIDBODY,
	COLLDIER,
	CONTROLLER,
	AUDIO_SOURCE,
	AUDIO_LISTENER,
	OUTLINE,
	CANVAS,
	UI_SPRITE,
	UI_TEXT,
	UI_BUTTON,
	EFFECT,
	SCRIPT,
	UPDATE_END,  // 업데이트 루프를 도는 컴포넌트의 끝. 빈 배열 하나를 차지하더라도 있는게 나은듯
    MESH_FILTER,
	FSM_STATE,
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
	virtual void Start() {};
	virtual void Tick()	= 0;
	virtual void FixedUpdate() = 0;
	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
    virtual void Draw(Camera* _camera) = 0;
	virtual void PostRender() = 0;
	// Editor Only
	virtual void EditorUpdate() = 0;
	virtual void EditorGlobalUpdate() {};
	virtual void EditorRender() = 0;
public:
	virtual void _CALLBACK OnEnable() {};
	virtual void _CALLBACK OnDisable() {};
	virtual void _CALLBACK OnDestroy() {};
public:
	inline eComponentType	GetType()				{ return mType; }
	inline void				SetActive(bool _active) { isActive = _active;}
	inline bool				IsActive()				{ return isActive; }
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) {};
public:
	Object* const	gameObject;
protected:
	eComponentType	mType;
	bool			isActive; 
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

class DynamicComponent
    : public Component
{

};
