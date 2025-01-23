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
	mRigidActor->release();
}

void Rigidbody::Start()
{
	gameObject->transform->UpdateMatrix();
	gameObject->transform->UpdatePxTransform();

	if (mRigidActor) return;
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

	EditorManager::mFocusViewport->GetWorldManager()->GetActiveWorld()->AddPxActor(mRigidActor);
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

}

void Rigidbody::Update()
{
	
}

void Rigidbody::PostUpdate()
{	
	//오브젝트 -> 리지드액터 동기화 
	gameObject->transform->UpdatePxTransform();

	mRigidActor->setGlobalPose(gameObject->transform->GetPxTransform());


	//PostUpdate가 모두 끝난 후, 여기서 simulate 함
}

void Rigidbody::PreRender()
{
	//리지드액터 -> 오브젝트 동기화 
	PxVec3 updatedPosition = mRigidActor->getGlobalPose().p;
	PxQuat updatedQuaternion = mRigidActor->getGlobalPose().q;

	//Position Freeze!
	if (mFreezePosition[0])
		updatedPosition[0] = gameObject->transform->position.x;
	if (mFreezePosition[1])
		updatedPosition[1] = gameObject->transform->position.y;
	if (mFreezePosition[2])
		updatedPosition[2] = gameObject->transform->position.z;


	//Rotation Freeze!
	if (mFreezeRotation[0])
	{
		Quaternion q = { updatedQuaternion.x,updatedQuaternion.y, updatedQuaternion.z, updatedQuaternion.w };
		Vector3 updatedRotation = q.ToEuler();
		
		if (mFreezeRotation[0])
			updatedRotation.x = gameObject->transform->rotation.x;
		if (mFreezeRotation[1])
			updatedRotation.y = gameObject->transform->rotation.y;
		if (mFreezePosition[2])
			updatedRotation.z = gameObject->transform->rotation.z;

		XMVECTOR quat = XMQuaternionRotationRollPitchYaw(updatedRotation.y, updatedRotation.z, updatedRotation.x );
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
	if (!mIsDynamic) return;

	PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(mRigidActor);
	rigid->setMass(mMass); 
}

json Rigidbody::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "Rigidbody";

	ret["isDynamic"] = mIsDynamic;
	ret["mass"] = mMass;
	return ret;
}

void Rigidbody::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	bool why = j.contains("isDynamic");
	mIsDynamic = j["isDynamic"].get<bool>();
	mMass = j["mass"].get<float>();
}

void Rigidbody::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Mass : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "Mass").c_str(), &mMass, 0.f, 0.f, 0.f))
	{
		SetMass(mMass);
	}

	ImGui::Separator();

	ImGui::Text("isDynamic : "); ImGui::SameLine;
	ImGui::Checkbox(("##isDynamic" + uid).c_str(), (bool*)&mIsDynamic);


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
