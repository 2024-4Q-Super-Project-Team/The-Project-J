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
#include "Component/Audio/AudioSource.h"
#include "Component/Audio/AudioListener.h"
#include "Component/Collider/Rigidbody.h"
#include "Component/Collider/BoxCollider.h"
#include "Component/Collider/SphereCollider.h"
#include "Component/Controller/PlayerController.h"
#include "Component/ParticleSystem/ParticleSystem.h"
#include "Component/BoneAttacher/BoneAttacher.h"
#include "Component/FSM/FiniteStateMachine.h"

#include "Interface/SaveBase.h"
#include "Editor/Interface/IEditorObject.h"

#include "Manager/GameManager.h"

class Component;

class Object
    : public Engine::Entity
    , public Engine::ICycleHandler
    , public Engine::SaveBase
	, public IEditorObject  
{
public:
    explicit Object(std::wstring_view _name, std::wstring_view _tag);
    virtual ~Object();
    Object(const Object& _other);
public:
    void Start();
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

    virtual void EditorUpdate()	override;
    void EditorGlobalUpdate();
    virtual void EditorRender()	override;
public:
    virtual void _CALLBACK OnEnable()  override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    // 대상 오브젝트와 그 오브젝트가 속한테이블을 받아야한다.
    void Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable);
public:
    // 컴포넌트를 생성한 후 반환하는 함수. 가변인자를 통해 추가적인 인자를 전달할 수 있음.
    template <class T>
    T* AddComponent(bool _isStart = true);
    // 특정 타입의 컴포넌트가 있으면 그 컴포넌트 중 제일 첫 번째 컴포넌트가 반환됨. 없으면 nullptr반환
    template <class T>
    T* EditorAddComponent();
    template <class T>
    T* GetComponent();
    // 특정 타입 컴포넌트를 배열로 전부 반환. 없으면 빈 배열이 반환됨.
    template <class T>
    std::vector<T*>& GetComponents();
    //가지고 있는 모든 컴포넌트를 반환합니다.
    std::vector<Component*> GetAllComponents();
public:
    void SetWorld(World* _world);
public:
    Transform* const transform;
protected:
    World* mOwnerWorld = nullptr;
    std::vector<Component*> mComponentArray[ComponentSize];
public:
    inline World* GetOwnerWorld() { return mOwnerWorld; }
public:
    json Serialize();
    void Deserialize(json& j);
public:
    bool isNodeOpen = false; // 하이라키뷰 트리노드가 열려있는지 선택된 여부
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};

template <class T>
T* Object::AddComponent(bool _isStart)
{
    static_assert(std::is_base_of<Component, T>::value, "AddComponent_Fail");
    T* component = new T(this);
    eComponentType type = component->GetType();
    int index = static_cast<UINT>(type);
    mComponentArray[index].push_back(component);

    if (GameManager::GetRunType() == eEngineRunType::GAME_MODE && _isStart)
    {
        // JSON_TODO : 이거 컴포넌트 추가도 Addcomponent로 하지 말아주세요 ㅠㅠ
        component->Start();
    }

    return component;
}

template<class T>
inline T* Object::EditorAddComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "AddComponent_Fail");
    T* component = new T(this);
    eComponentType type = component->GetType();
    int index = static_cast<UINT>(type);
    mComponentArray[index].push_back(component);

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