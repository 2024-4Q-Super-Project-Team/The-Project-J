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
    // ��� ������Ʈ�� �� ������Ʈ�� �������̺��� �޾ƾ��Ѵ�.
    void Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable);
public:
    // ������Ʈ�� ������ �� ��ȯ�ϴ� �Լ�. �������ڸ� ���� �߰����� ���ڸ� ������ �� ����.
    template <class T>
    T* AddComponent(bool _isStart = true);
    // Ư�� Ÿ���� ������Ʈ�� ������ �� ������Ʈ �� ���� ù ��° ������Ʈ�� ��ȯ��. ������ nullptr��ȯ
    template <class T>
    T* EditorAddComponent();
    template <class T>
    T* GetComponent();
    // Ư�� Ÿ�� ������Ʈ�� �迭�� ���� ��ȯ. ������ �� �迭�� ��ȯ��.
    template <class T>
    std::vector<T*>& GetComponents();
    //������ �ִ� ��� ������Ʈ�� ��ȯ�մϴ�.
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
    bool isNodeOpen = false; // ���̶�Ű�� Ʈ����尡 �����ִ��� ���õ� ����
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
        // JSON_TODO : �̰� ������Ʈ �߰��� Addcomponent�� ���� �����ּ��� �Ф�
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
    static std::vector<T*> result; // ��ȯ�� ��� �����̳�
    result.clear(); // ���� ������ �ʱ�ȭ

    for (auto& compVec : mComponentArray)
    {
        for (auto* comp : compVec)
        {
            T* temp = dynamic_cast<T*>(comp); // Ÿ�� ĳ����
            if (temp)
            {
                result.push_back(temp); // ��ġ�ϴ� Ÿ���̸� �߰�
            }
        }
    }
    return result;
}