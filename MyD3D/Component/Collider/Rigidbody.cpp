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
	//������Ʈ -> ��������� ����ȭ 
	gameObject->transform->UpdatePxTransform();

	mRigidActor->setGlobalPose(gameObject->transform->GetPxTransform());


	//PostUpdate�� ��� ���� ��, ���⼭ simulate ��
}

void Rigidbody::PreRender()
{
	//��������� -> ������Ʈ ����ȭ 
	gameObject->transform->UpdateFromPxTransform(mRigidActor->getGlobalPose());
}

void Rigidbody::Render()
{
	//������Ʈ�� ���� ����� �� ���� 
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
