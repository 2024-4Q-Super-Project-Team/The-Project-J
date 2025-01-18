#include "pch.h"
#include "Rigidbody.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"

Rigidbody::Rigidbody(Object* _owner) :Component(_owner)
{
	mType = eComponentType::RIGIDBODY;
	_owner->transform->UpdateMatrix();
	_owner->transform->UpdatePxTransform();
	mRigidActor = nullptr;
}

Rigidbody::~Rigidbody()
{
	mRigidActor->release();
}

void Rigidbody::Start()
{
	//TODO : Rigibody 상속받아서 dynamic이랑 아닌거 클래스 따로 만들어서 생성때부터 정해지게 합시다. 
	if (mIsDynamic == false)
	{
		mRigidActor = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidStatic(gameObject->transform->GetPxTransform());
	}
	else
	{
		mRigidActor = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidDynamic(gameObject->transform->GetPxTransform());
	}
	mRigidActor->userData = gameObject;

	GameManager::GetCurrentWorld()->AddPxActor(mRigidActor);

	mRigidActor->setGlobalPose(gameObject->transform->GetPxTransform());
}

void Rigidbody::Tick()
{
}

void Rigidbody::FixedUpdate()
{
}

void Rigidbody::PreUpdate()
{
	//오브젝트 -> 리지드액터 동기화 
	mRigidActor->setGlobalPose(gameObject->transform->GetPxTransform());
}

void Rigidbody::Update()
{
}

void Rigidbody::PostUpdate()
{
	//리지드액터 -> 오브젝트 동기화 
	gameObject->transform->UpdateFromPxTransform(mRigidActor->getGlobalPose());
}

void Rigidbody::PreRender()
{
}

void Rigidbody::Render()
{
}

void Rigidbody::Draw(Camera* _camera)
{
}

void Rigidbody::PostRender()
{
}

void Rigidbody::SetMass(float mass)
{
	if (!mIsDynamic) return;

	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->setMass(mass);
}

json Rigidbody::Serialize()
{
	return json();
}

void Rigidbody::Deserialize(json& j)
{
}
