#include "pch.h"
#include "Object.h"
#include "ObjectGroup/ObjectGroup.h"

Object::Object(std::wstring_view _name, std::wstring_view _tag)
	: Engine::Entity(_name, _tag)
    , transform(new Transform(this))
	, mOwnerGroup(nullptr)
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)].push_back(transform);
}

Object::~Object()
{
	for (auto& compArr : mComponentArray)
	{
        SAFE_DELETE_VECTOR(compArr);
	}
}

Object::Object(const Object& _other)
    : Engine::Entity(_other.mName, _other.mTag)
    , transform(new Transform(this))
    , mOwnerGroup(_other.mOwnerGroup)
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)][0] = transform;
}

void Object::Tick()
{
    for(int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
	{
		for (auto& comp : mComponentArray[i])
		{
			if (comp->IsActive())
				comp->Tick();
		}
	}
}

void Object::FixedUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->FixedUpdate();
        }
    }
}

void Object::PreUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PreUpdate();
        }
    }
}

void Object::Update()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Update();
        }
    }
}

void Object::PostUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PostUpdate();
        }
    }
}

void Object::PreRender()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PreRender();
        }
    }
    if(!transform->GetParent())
        transform->UpdateMatrix();
}

void Object::Render()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Render();
        }
    }
}

void Object::Draw(Camera* _camera)
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Draw(_camera);
        }
    }
}

void Object::PostRender()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PostRender();
        }
    }
}

void _CALLBACK Object::OnEnable()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetActive(true);
    }
	for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
	{
		if (comp->IsActive())
			static_cast<MonoBehaviour*>(comp)->OnEnable();
	}
	for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
	{
		if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnEnable();
	}
	return void _CALLBACK();
}

void _CALLBACK Object::OnDisable()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetActive(false);
    }
	for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
	{
		if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnDisable();
	}
	for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
	{
		if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnDisable();
	}
	return void _CALLBACK();
}

void _CALLBACK Object::OnDestroy()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetDestroy();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
    {
        static_cast<MonoBehaviour*>(comp)->OnDestroy();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
    {
        static_cast<MonoBehaviour*>(comp)->OnDestroy();
    }
	return void _CALLBACK();
}

void Object::Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable)
{
    for (auto& compArr : mComponentArray)
    {
        for (auto& comp : compArr)
        {
            comp->Clone(_pDest, _objTable);
        }
    }
}

nlohmann::json Object::Serialize()
{
    nlohmann::json ret;
    ret += nlohmann::json{ "parent", transform->GetParent()->gameObject->GetName() };
    ret += nlohmann::json{ "pos.x", transform->GetParent()->position.x };
    ret += nlohmann::json{ "pos.y", transform->GetParent()->position.y };
    ret += nlohmann::json{ "pos.z", transform->GetParent()->position.z };
    ret += nlohmann::json{ "rot.x", transform->GetParent()->rotation.x };
    ret += nlohmann::json{ "rot.y", transform->GetParent()->rotation.y };
    ret += nlohmann::json{ "rot.z", transform->GetParent()->rotation.z };
    ret += nlohmann::json{ "rot.w", transform->GetParent()->rotation.w };
    ret += nlohmann::json{ "scale.x", transform->GetParent()->scale.x };
    ret += nlohmann::json{ "scale.y", transform->GetParent()->scale.y };
    ret += nlohmann::json{ "scale.z", transform->GetParent()->scale.z };

    for (size_t i = 0; i < ComponentSize; i++)
    {
        size_t size = mComponentArray[i].size();
        for (size_t j = 0; j < size; j++)
        {
            ret += mComponentArray[i][j]->Serialize();
        }
    }
    return ret;
}
