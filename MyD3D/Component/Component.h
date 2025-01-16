#pragma once
#include "Interface/ICycleHandler.h"
#include "Interface/SaveBase.h"


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
	UPDATE_END,  // 업데이트 루프를 도는 컴포넌트의 끝. 빈 배열 하나를 차지하더라도 있는게 나은듯
    MESH_FILTER,
    SIZE,
};

constexpr static UINT ComponentSize = static_cast<UINT>(eComponentType::SIZE);

class Component
	: public Engine::ICycleHandler
	, public Engine::SaveBase
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
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) {};

	void AddField(Serial* _serial)
	{
		mSerials.push_back(_serial);
	}

	virtual json Serialize() = 0;
	virtual void Deserialize(json& j) = 0;
public:
	Object* const	gameObject;
protected:
	eComponentType	mType;
	bool			isActive; 
private:
	bool			mIsAwake = false; //깨어났는지를 나타냅니다. Start함수 호출 여부와 같습니다. 

	std::vector<Serial*> mSerials;
public:
    virtual void EditorRendering();
};

class DynamicComponent
    : public Component
{

};

struct Serial {
	Serial(std::string_view _key)
		: key(_key) {
	}
	std::string key;
	Editor::Widget* widget = nullptr;
};

template <typename T>
struct SerialData : public Serial
{
	T val;

	SerialData(std::string_view _name, Component* mono)
		: Serial(_name)
	{
		mono->AddField(this);
		if (std::is_same<T, Vector3>::value)
			widget = new Editor::InputVector3(_name.data(), &val);
	}
};

#ifdef _DEBUG
#define SerializeField(Type, Name)\
	SerialData<Type> Name##Data = SerialData<Type>(#Name, this);\
	Type Name
#else
#define SerializeField(Type, Name)\
	Type Name
#endif