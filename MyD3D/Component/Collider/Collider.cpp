#include "pch.h"
#include "Collider.h"
#include "Component/Collider/Rigidbody.h"
#include "Physics/PhysicsManager.h"

bool Collider::bDrawMode = false;

Collider::Collider(Object* _owner) : Component(_owner)
{
	mType = eComponentType::COLLDIER;
	mIsTrigger = false;

	const auto& materials = GameManager::GetPhysicsManager()->GetMaterials();
	for (auto& material : materials)
	{
		mMaterials.push_back(material.first.c_str());
	}
}

Collider::~Collider()
{
	if (mShape)
	{
		mShape->release();
	}
}

void Collider::Start()
{
	if (mShape)
	{
		AddShapeToRigidbody();
		SetMaterial(mMaterials[mMaterialIdx]);
		mShape->userData = this;
	}
}

void Collider::Tick()
{
}

void Collider::FixedUpdate()
{
}

void Collider::PreUpdate()
{
	CheckTriggers();
}

void Collider::Update()
{
	
}

void Collider::PostUpdate()
{
}

void Collider::PreRender()
{
}

void Collider::Render()
{
}

void Collider::Draw(Camera* _camera)
{
}

void Collider::PostRender()
{
}

void Collider::EditorUpdate()
{
}

void Collider::EditorRender()
{
}

Vector3 Collider::GetDistanceFromCamera(Camera* _camera)
{
	return _camera->GetDistance(gameObject->transform);
}


void Collider::AddShapeToRigidbody()
{
	if (addedToRigid) return; 
	if (mShape == nullptr) return;

	mRefRigidbody = gameObject->GetComponent<Rigidbody>();
	if (mRefRigidbody)
		mRefRigidbody->AddShape(mShape);

	SetIsTrigger();
	SetPosition();
	SetRotation();

	mShape->userData = this;
	addedToRigid = true;
}


void Collider::DrawWire()
{
}

void Collider::AddTriggerOther(Collider* _collider)
{
	mTriggerOthers.insert(_collider);
}

void Collider::RemoveTriggerOther(Collider* _collider)
{
	if (mTriggerOthers.find(_collider) != mTriggerOthers.end())
		mTriggerOthers.erase(_collider);
}

void Collider::SetIsTrigger()
{
	if (mIsTrigger == true)
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false); //충돌 반응 비활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true); // 트리거 설정
	}
	else
	{
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); //충돌 반응 활성화 
		mShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // 트리거 설정
	}
}


void Collider::SetMaterial(std::string _name)
{
	PxMaterial* material = GameManager::GetPhysicsManager()->GetMaterial(_name);
	mShape->setMaterials(&material, 1);
}

void Collider::CheckTriggers()
{
	auto triggerScripts = gameObject->GetComponents<MonoBehaviour>();

	for (Collider* col : mTriggerOthers)
	{
		auto otherScripts = col->gameObject->GetComponents<MonoBehaviour>();

		for (auto script : triggerScripts)
			script->OnTriggerStay(this, col);
		for (auto script : otherScripts)
			script->OnTriggerStay(col, this);
	}
}