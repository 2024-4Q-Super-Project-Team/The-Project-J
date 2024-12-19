#include "pch.h"
#include "Object.h"
#include "Component/Script/Script.h"
#include "ObjectGroup/ObjectGroup.h"

GameObject::GameObject(ObjectGroup* _owner, std::wstring_view _name, std::wstring_view _tag)
	: transform(new Transform(this))
	, mOwnerGroup(_owner)
	, mParent(nullptr)
	, mRootParent(nullptr)
{
}

GameObject::~GameObject()
{
	for (auto& compArr : mComponentArray)
	{
		SAFE_DELETE_VECTOR(compArr)
	}
	SAFE_DELETE_VECTOR(mChildren)
	delete transform;
}

void GameObject::Tick()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->Tick();
		}
	}
}

void GameObject::FixedUpdate()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->FixedUpdate();
		}
	}
}

void GameObject::PreUpdate()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->PreUpdate();
		}
	}
}

void GameObject::Update()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->Update();
		}
	}
}

void GameObject::PostUpdate()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->PostUpdate();
		}
	}
}

void GameObject::PreRender()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->PreRender();
		}
	}
}

void GameObject::Render(GraphicsManager* _graphicsManager)
{
	//if (transform)
	//{
	//	transform->CalculateMatrix();
	//}
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->Render(_graphicsManager);
		}
	}
}

void GameObject::PostRender()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			if (comp->IsActive())
				comp->PostRender();
		}
	}
}

void _CALLBACK GameObject::OnEnable()
{
	using namespace Component;
	for (auto comp : mComponentArray[(UINT)ComponentType::Script])
	{
		if(comp->IsActive())
			dynamic_cast<Engine::MonoBehaviour*>(comp)->OnEnable();
	}
	for (auto comp : mComponentArray[(UINT)ComponentType::FiniteStateMachine])
	{
		if (comp->IsActive())
			dynamic_cast<Engine::MonoBehaviour*>(comp)->OnEnable();
	}
	return void _CALLBACK();
}

void _CALLBACK GameObject::OnDisable()
{
	return void _CALLBACK();
}

void _CALLBACK GameObject::OnDestroy()
{
	const std::vector<Transform*> children = transform->GetChildren();
	for (auto child : children)
	{
		child->GetOwner<GameObject>()->SetDestroy();
	}
	return void _CALLBACK();
}

GameObject* GameObject::CreateChild(std::wstring_view _name)
{
	GameObject* instance = new GameObject(mOwnerGroup, _name, mTag);
	mChildren.push_back(instance);
	instance->mState = EntityState::Active;
	instance->mParent = this;
	instance->mRootParent = mRootParent;
	return nullptr;
}
