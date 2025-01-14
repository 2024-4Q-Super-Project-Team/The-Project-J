#pragma once
#include "Interface/ICycleHandler.h"
#include "Helper/Entity/Entity.h"
#include "Component/Transform/transform.h"
#include "Component/Camera/Camera.h"
#include "Component/Light/Light.h"
#include "Component/MonoBehaviour/MonoBehaviour.h"
#include "Component/Renderer/SkinnedMeshRenderer/SkinnedMeshRenderer.h"
#include "Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "Component/Animator/Animator.h"
#include "Component/EditorUI/EditorUI.h"
#include "Interface/IGUID.h"
class Component;
class ObjectGroup;

class Object
	: public Engine::Entity
    , public Engine::ICycleHandler
    , public Engine::IGUID
{
public:
	explicit Object(std::wstring_view _name, std::wstring_view _tag);
	virtual ~Object();
    Object(const Object& _other);
public:
    virtual void Tick()			override;
    virtual void FixedUpdate()	override;
    virtual void PreUpdate()	override;
    virtual void Update()		override;
    virtual void PostUpdate()	override;
    virtual void PreRender()	override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera);
    virtual void PostRender()	override;
private:
	virtual void _CALLBACK OnEnable()  override;
	virtual void _CALLBACK OnDisable() override;
	virtual void _CALLBACK OnDestroy() override;
public:
    // 대상 오브젝트와 그 오브젝트가 속한테이블을 받아야한다.
    void Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable);
public:
    // 컴포넌트를 생성한 후 반환하는 함수. 가변인자를 통해 추가적인 인자를 전달할 수 있음.
	template <class T, typename... Args>
	T* AddComponent(Args&&... args);
    // 특정 타입의 컴포넌트가 있으면 그 컴포넌트 중 제일 첫 번째 컴포넌트가 반환됨. 없으면 nullptr반환
	template <class T>
	T* GetComponent();
    // 특정 타입 컴포넌트를 배열로 전부 반환. 없으면 빈 배열이 반환됨.
    template <class T>
    std::vector<T*>& GetComponents();
    // 해당 컴포넌트의 속성 값을 복사하여 추가합니다. 해당 컴포넌트의 복사 생성자가 정의되어 있어야 함.
    template <class T>
    T* CloneComponent(T* _pSrc);

    //에디터 전용 컴포넌트 추가 함수입니다. 
    template <class T, typename... Args>
    T* EditorAddComponent(Args&&... args);

public:
  	json Serialize();
    json SerializeComponents();
    void Deserialize(json& j);
public:
    Transform* const transform;
public:
    inline ObjectGroup* GetOwnerObjectGroup() { return mOwnerGroup; }
    inline void SetOwnerGroup(ObjectGroup* _pObjectGroup) { mOwnerGroup = _pObjectGroup; }
protected:
    ObjectGroup* mOwnerGroup;
	std::vector<Component*> mComponentArray [ComponentSize];
public:
    void EditorRendering()
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        {   // 활성화 / 비활성화
            std::string label;
            label.assign(mName.begin(), mName.end());
            label += uid;
            bool preState = mState == EntityState::Active ? true : false;
            bool isActive = preState;
            ImGui::Checkbox(label.c_str(), &isActive);
            if (isActive != preState)
            {
                isActive == true ? OnEnable() : OnDisable();
                mState = isActive == true ? EntityState::Active : EntityState::Passive;
            }
        }
    }
    friend class Editor::Inspector;
};

template <class T, typename... Args>
T* Object::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>::value, "AddComponent_Fail");
	T* component = new T(this, std::forward<Args>(args)...);
    eComponentType type = component->GetType();
    int index = static_cast<UINT>(type);
	mComponentArray[index].push_back(component);
	component->Start();
	return component;
}

template <class T>
T* Object::GetComponent()
{
	for (auto& compVec : mComponentArray)
	{
		for (auto& comp : compVec)
		{
			T* temp = dynamic_cast<T*>(comp);
			if (temp) return temp;
		}
	}
	return nullptr;
}

template <class T>
std::vector<T*>& Object::GetComponents()
{
	static std::vector<T*> result; // 반환할 결과 컨테이너
	result.clear(); // 이전 데이터 초기화

	for (auto& compVec : mComponentArray)
	{
		for (auto* comp : compVec)
		{
			T* temp = dynamic_cast<T*>(comp); // 타입 캐스팅
			if (temp)
			{
				result.push_back(temp); // 일치하는 타입이면 추가
			}
		}
	}
	return result;
}

template<class T>
inline T* Object::CloneComponent(T* _pSrc)
{

    return nullptr;
}

template <class T, typename... Args>
T* Object::EditorAddComponent(Args&&... args)
{
    static_assert(std::is_base_of<Component, T>::value, "EditorAddComponent_Fail");
    T* component = new T(this, std::forward<Args>(args)...);
    eComponentType type = component->GetType();
    int index = static_cast<UINT>(type);
    if (type == eComponentType::SCRIPT)
        component->SetActive(false);
    else
        component->SetActive(true);
    mComponentArray[index].push_back(component);
    
    return component;
}
