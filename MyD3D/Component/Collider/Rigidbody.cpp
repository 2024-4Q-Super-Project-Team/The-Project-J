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
	//TODO : Rigibody ��ӹ޾Ƽ� dynamic�̶� �ƴѰ� Ŭ���� ���� ���� ���������� �������� �սô�. 
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
	//������Ʈ -> ��������� ����ȭ 
	mRigidActor->setGlobalPose(gameObject->transform->GetPxTransform());
}

void Rigidbody::Update()
{
}

void Rigidbody::PostUpdate()
{
	//��������� -> ������Ʈ ����ȭ 
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
	mMass = mass;
}

json Rigidbody::Serialize()
{
	json ret;


	return ret;
}

void Rigidbody::Deserialize(json& j)
{
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

		ImGui::Text("isTrigger : "); ImGui::SameLine;
		if (ImGui::Checkbox(("##isDynamic" + uid).c_str(), (bool*)&mIsDynamic))
		{
			//TODO
		}
		

		ImGui::TreePop();
	}
}
