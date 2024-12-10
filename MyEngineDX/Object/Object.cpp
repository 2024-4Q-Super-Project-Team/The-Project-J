#include "pch.h"
#include "Object.h"

GameObject::GameObject(ObjectGroup* _owner)
	: transform(new Transform3D())
	, mOwnerGroup(_owner)
{
}

GameObject::GameObject(ObjectGroup* _owner, std::wstring_view _name, std::wstring_view _tag)
	: transform(new Transform3D())
	, mOwnerGroup(_owner)
{
}

GameObject::~GameObject()
{
	for (auto& compArr : mComponentArray)
	{
		for (auto& comp : compArr)
		{
			SAFE_DELETE(comp)
		}
	}
	delete transform;
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
	if (transform)
	{
		transform->CalculateMatrix();
	}
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
	return void _CALLBACK();
}
