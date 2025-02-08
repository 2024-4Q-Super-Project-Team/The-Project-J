#include "pch.h"
#include "Rigidbody.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
#include "ViewportScene/ViewportScene.h"
#include "World/WorldManager.h"

Rigidbody::Rigidbody(Object* _owner) :Component(_owner)
{
	SetEID("Rigidbody");
	mType = eComponentType::RIGIDBODY;
	mRigidActor = nullptr;
}

Rigidbody::~Rigidbody()
{
	if (mRigidActor)
	{
		gameObject->GetOwnerWorld()->RemovePxActor(mRigidActor);
		mRigidActor->release();
	}
	
}

void Rigidbody::Start()
{
	if (mIsDynamic == false)
	{
		mRigidActor = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidStatic(gameObject->transform->GetPxWorldTransform());
	}
	else
	{
		PxRigidDynamic* rigidDynamic = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidDynamic(gameObject->transform->GetPxWorldTransform());

		rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, true);
		mRigidActor = rigidDynamic;
	}

	
	mRigidActor->setGlobalPose(gameObject->transform->GetPxWorldTransform());
	
	gameObject->GetOwnerWorld()->AddPxActor(mRigidActor);

	SetMass(mMass);
	SetIsKinematic(mIsKinematic);

	mRigidActor->userData = this;
}

void Rigidbody::Tick()
{
}

void Rigidbody::FixedUpdate()
{
}

void Rigidbody::PreUpdate()
{
	CheckContacts();
}

void Rigidbody::Update()
{
}

void Rigidbody::PostUpdate()
{	
	gameObject->transform->UpdatePxTransform();
	mRigidActor->setGlobalPose(gameObject->transform->GetPxWorldTransform());

	//PostUpdate가 모두 끝난 후, 여기서 simulate 함
}

void Rigidbody::PreRender()
{
	//오브젝트 -> 리지드액터 동기화 
	gameObject->transform->UpdateFromPxTransform(mRigidActor->getGlobalPose());
	
	PxVec3 updatedPosition = mRigidActor->getGlobalPose().p;
	PxQuat updatedQuaternion = mRigidActor->getGlobalPose().q;
	
	//Position Freeze!
	if (mFreezePosition[0])
		updatedPosition[0] = gameObject->transform->GetWorldPosition().x;
	if (mFreezePosition[1])
		updatedPosition[1] = gameObject->transform->GetWorldPosition().y;
	if (mFreezePosition[2])
		updatedPosition[2] = gameObject->transform->GetWorldPosition().z;
	
	
	//Rotation Freeze!

	if (mFreezeRotation[0] || mFreezeRotation[1] || mFreezeRotation[2])
	{
		Quaternion q = { updatedQuaternion.x,updatedQuaternion.y, updatedQuaternion.z, updatedQuaternion.w };
		Vector3 updatedRotation = q.ToEuler();

		if (mFreezeRotation[0])
			updatedRotation.x = gameObject->transform->GetWorldRotation().x;
		if (mFreezeRotation[1])
			updatedRotation.y = gameObject->transform->GetWorldRotation().y;
		if (mFreezeRotation[2])
			updatedRotation.z = gameObject->transform->GetWorldRotation().z;

		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(updatedRotation.y, updatedRotation.x, updatedRotation.z);
		XMStoreFloat4(&q, quat);
		updatedQuaternion = PxQuat(q.x, q.y, q.z, q.w);

	}

	PxTransform finalTransform(updatedPosition, updatedQuaternion);
	gameObject->transform->UpdateFromPxTransform(finalTransform);
}

void Rigidbody::Render()
{
	//오브젝트의 값이 변경될 수 있음 
}

void Rigidbody::Draw(Camera* _camera)
{
}

void Rigidbody::PostRender()
{

}

void Rigidbody::EditorUpdate()
{
}

void Rigidbody::EditorRender()
{
}


void Rigidbody::SetMass(float mass)
{
	if (!mRigidActor || !mIsDynamic) return;
	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->setMass(mMass); 
}

void Rigidbody::SetIsKinematic(bool b)
{
	if (!mRigidActor || !mIsDynamic) return;
	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, b);
}

void Rigidbody::SetIsDynamic(bool _isDynamic)
{
	mIsDynamic = _isDynamic;

	PxActor* prevActor = mRigidActor;
	if (mIsDynamic == false)
	{
		mRigidActor = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidStatic(gameObject->transform->GetPxWorldTransform());
	}
	else
	{
		mRigidActor = GameManager::GetPhysicsManager()->GetPhysics()
			->createRigidDynamic(gameObject->transform->GetPxWorldTransform());
	}

	gameObject->GetOwnerWorld()->AddPxActor(mRigidActor);
	gameObject->GetOwnerWorld()->RemovePxActor(prevActor);

	SetMass(mMass);
	SetIsKinematic(mIsKinematic);

	auto& colliders = gameObject->GetComponents<Collider>();
	for (auto& collider : colliders)
	{
		collider->AddShapeToRigidbody();
	}
	mRigidActor->userData = this;
}

void Rigidbody::SetDisableGravity(bool b)
{
	if (!mRigidActor) return;
	mRigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, b);
	
	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->setLinearVelocity(PxVec3(0, 0, 0));
}

void Rigidbody::SetFreezePosition(bool bx, bool by, bool bz)
{
	mFreezePosition[0] = bx;
	mFreezePosition[1] = by;
	mFreezePosition[2] = bz;
}

void Rigidbody::SetFreezeRotation(bool bx, bool by, bool bz)
{
	mFreezeRotation[0] = bx;
	mFreezeRotation[1] = by;
	mFreezeRotation[2] = bz;
}

void Rigidbody::AddForce(Vector3 force, PxForceMode::Enum forceMode)
{
	if (!mRigidActor || !mIsDynamic) return;

	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->addForce(PxVec3(force.x, force.y, force.z), forceMode);
}

void Rigidbody::AddContactOther(Rigidbody* _rigid)
{
	mContactOthers.insert(_rigid);
}

void Rigidbody::RemoveContactOther(Rigidbody* _rigid)
{
	if (mContactOthers.find(_rigid) != mContactOthers.end())
		mContactOthers.erase(_rigid);
}

json Rigidbody::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "Rigidbody";

	ret["isDynamic"] = mIsDynamic;
	ret["isKinematic"] = mIsKinematic;
	ret["mass"] = mMass;
	ret["freeze position"] = { mFreezePosition[0], mFreezePosition[1], mFreezePosition[2] };
	ret["freeze rotation"] = { mFreezeRotation[0], mFreezeRotation[1], mFreezeRotation[2] };
	return ret;
}

void Rigidbody::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("isDynamic"))
		mIsDynamic = j["isDynamic"].get<bool>();
	if (j.contains("isKinematic"))
		mIsKinematic = j["isKinematic"].get<bool>();
	if (j.contains("mass"))
		mMass = j["mass"].get<float>();

	if (j.contains("freeze position"))
	{
		mFreezePosition[0] = j["freeze position"][0];
		mFreezePosition[1] = j["freeze position"][1];
		mFreezePosition[2] = j["freeze position"][2];

		mFreezeRotation[0] = j["freeze rotation"][0];
		mFreezeRotation[1] = j["freeze rotation"][1];
		mFreezeRotation[2] = j["freeze rotation"][2];
	}
}

void Rigidbody::CheckContacts()
{
	auto triggerScripts = gameObject->GetComponents<MonoBehaviour>();

	for (Rigidbody* rigid : mContactOthers)
	{
		auto otherScripts = rigid->gameObject->GetComponents<MonoBehaviour>();

		for (auto script : triggerScripts)
			script->OnCollisionStay(this, rigid);
		for (auto script : otherScripts)
			script->OnCollisionStay(rigid, this);
	}
}

void Rigidbody::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Separator();

	ImGui::Text("isDynamic: "); ImGui::SameLine;
	ImGui::Checkbox(("##isDynamic" + uid).c_str(), (bool*)&mIsDynamic);

	if(mIsDynamic)
	{
		ImGui::Text("isKinematic: "); ImGui::SameLine;
		if (ImGui::Checkbox(("##isKinematic" + uid).c_str(), (bool*)&mIsKinematic))
		{
			SetIsKinematic(mIsKinematic);
		}

		ImGui::Text("Mass : "); ImGui::SameLine;
		if (ImGui::DragFloat((uid + "Mass").c_str(), &mMass, 0.f, 0.f, 1000.f))
		{
			SetMass(mMass);
		}
	}

	ImGui::Separator();

	ImGui::Text("disable gravity: "); ImGui::SameLine;
	if (ImGui::Checkbox(("##disable gravity" + uid).c_str(), (bool*)&mDisableGravity))
	{
		SetDisableGravity(mDisableGravity);
	}

	ImGui::Separator();

	ImGui::Text("Freeze Position : "); ImGui::SameLine(); ImGui::Text("x "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezePositionX").c_str(), (bool*)&mFreezePosition[0]));
	ImGui::SameLine(); ImGui::Text("y "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezePositionY").c_str(), (bool*)&mFreezePosition[1]));
		ImGui::SameLine(); ImGui::Text("z "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezePositionZ").c_str(), (bool*)&mFreezePosition[2]));

	ImGui::Text("Freeze Rotation : "); ImGui::SameLine(); ImGui::Text("x "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezeRotationX").c_str(), (bool*)&mFreezeRotation[0]));
	ImGui::SameLine(); ImGui::Text("y "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezeRotationY").c_str(), (bool*)&mFreezeRotation[1]));
	ImGui::SameLine(); ImGui::Text("z "); ImGui::SameLine();
	if (ImGui::Checkbox((uid + "FreezeRotationnZ").c_str(), (bool*)&mFreezeRotation[2]));

}
