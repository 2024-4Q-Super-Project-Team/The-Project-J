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

#include "Interface/SaveBase.h"
#include "Editor/Interface/IEditorObject.h"

class Component;
class ObjectGroup;

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
    virtual void EditorRender()	override;
private:
    virtual void _CALLBACK OnEnable()  override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    // ��� ������Ʈ�� �� ������Ʈ�� �������̺��� �޾ƾ��Ѵ�.
    void Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable);
public:
    // ������Ʈ�� ������ �� ��ȯ�ϴ� �Լ�. �������ڸ� ���� �߰����� ���ڸ� ������ �� ����.
    template <class T, typename... Args>
    T* AddComponent(Args&&... args);
    // Ư�� Ÿ���� ������Ʈ�� ������ �� ������Ʈ �� ���� ù ��° ������Ʈ�� ��ȯ��. ������ nullptr��ȯ
    template <class T>
    T* GetComponent();
    // Ư�� Ÿ�� ������Ʈ�� �迭�� ���� ��ȯ. ������ �� �迭�� ��ȯ��.
    template <class T>
    std::vector<T*>& GetComponents();
    //������ �ִ� ��� ������Ʈ�� ��ȯ�մϴ�.
    std::vector<Component*> GetAllComponents();
    // �ش� ������Ʈ�� �Ӽ� ���� �����Ͽ� �߰��մϴ�. �ش� ������Ʈ�� ���� �����ڰ� ���ǵǾ� �־�� ��.
    template <class T>
    T* CloneComponent(T* _pSrc);

public:
  	json Serialize();
    void Deserialize(json& j);
public:
    Transform* const transform;
public:
    inline ObjectGroup* GetOwnerObjectGroup() { return mOwnerGroup; }
    inline void SetOwnerGroup(ObjectGroup* _pObjectGroup) { mOwnerGroup = _pObjectGroup; }
protected:
    ObjectGroup* mOwnerGroup;
    std::vector<Component*> mComponentArray[ComponentSize];
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};

template <class T, typename... Args>
T* Object::AddComponent(Args&&... args)
{
    static_assert(std::is_base_of<Component, T>::value, "AddComponent_Fail");
    T* component = new T(this, std::forward<Args>(args)...);
    eComponentType type = component->GetType();
    int index = static_cast<UINT>(type);
    mComponentArray[index].push_back(component);
    // Start�� ���߿� ȣ��
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

template<class T>
inline T* Object::CloneComponent(T* _pSrc)
{

    return nullptr;
}
