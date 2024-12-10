#pragma once
#include "Component\Script\Script.h"

class ObjectGroup;

class GameObject
	: public Engine::IEngineCycle
	, public Engine::Entity
{
	using ComponentArray = std::array<std::vector<Component::ComponentBase*>, static_cast<int>(Component::ComponentType::SIZE)>;
public:
	GameObject(ObjectGroup* _owner);
	GameObject(ObjectGroup* _owner, std::wstring_view _name, std::wstring_view _tag);
	virtual ~GameObject();
	GameObject(const GameObject& _other) = default;
	GameObject(GameObject&& _other) noexcept = default;
	GameObject& operator=(const GameObject& _other) = default;
	GameObject& operator=(GameObject&& _other) noexcept = default;
public:
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render(GraphicsManager* _graphicsManager) override;
	virtual void PostRender() override;
private:
	virtual void _CALLBACK OnEnable()  override;
	virtual void _CALLBACK OnDisable() override;
	virtual void _CALLBACK OnDestroy() override;
public:
	template <class T, typename... Args>
	T* AddComponent(Args&&... args);
	template <class T>
	T* GetComponent();
	template <class T>
    std::vector<T*>& GetComponents();
public:
	Transform3D* const transform;
private:
	ComponentArray	mComponentArray;
	ObjectGroup*	mOwnerGroup;
public:
	ObjectGroup* GetOwnerGroup() { return mOwnerGroup; }

};


template <class T, typename... Args>
T* GameObject::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component::ComponentBase, T>::value, "AddComponent_Fail");
	T* component = new T(this, std::forward<Args>(args)...);
	Component::ComponentType type = component->GetType();
	int index = static_cast<int>(type);
	mComponentArray[index].push_back(component);
	component->Start();
	return component;
}

template <class T>
T* GameObject::GetComponent()
{
	for (auto& compVec : mComponentArray)
	{
		for(auto& comp : compVec)
		{
			T* temp = dynamic_cast<T*>(comp);
			if (temp) return temp;
		}
	}
	return nullptr;
}

template <class T>
std::vector<T*>& GameObject::GetComponents()
{
	static std::vector<T*> result; // 반환할 결과 컨테이너
	result.clear(); // 이전 데이터 초기화

	for (auto& compVec : mComponentArray)
	{
		for (auto* comp : compVec)
		{
			T* castedComp = dynamic_cast<T*>(comp); // 타입 캐스팅
			if (castedComp)
			{
				result.push_back(castedComp); // 일치하는 타입이면 추가
			}
		}
	}
	return result;
}
