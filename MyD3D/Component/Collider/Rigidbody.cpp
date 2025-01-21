#include "pch.h"
#include "Rigidbody.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
#include "ViewportScene/ViewportScene.h"
#include "World/WorldManager.h"

Rigidbody::Rigidbody(Object* _owner) :Component(_owner)
{
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
	gameObject->transform->UpdateFromPxTransform(mRigidActor->getGlobalPose());
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
	if (ImGui::TreeNodeEx(("Rigidbody" + uid).c_str(), EDITOR_FLAG_MAIN))
	{
		ImGui::Separator();

		ImGui::Text("Mass : "); ImGui::SameLine;
		if (ImGui::DragFloat((uid + "Mass").c_str(), &mMass, 0.f, 0.f, 0.f))
		{
			SetMass(mMass);
		}

		ImGui::Separator();

		ImGui::Text("isDynamic : "); ImGui::SameLine;
		ImGui::Checkbox(("##isDynamic" + uid).c_str(), (bool*)&mIsDynamic);

		

		ImGui::TreePop();
	}
}
