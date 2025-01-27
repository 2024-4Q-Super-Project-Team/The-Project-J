#include "pch.h"
#include "PlayerController.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
#include "World/WorldManager.h"
#include "ViewportScene/ViewportScene.h"

PxControllerManager* PlayerController::ControllerManager = nullptr;

PlayerController::PlayerController(Object* _owner) :Component(_owner)
{
	SetEID("PlayerController");
	mType = eComponentType::CONTROLLER;

	if (ControllerManager == nullptr)
		ControllerManager = PxCreateControllerManager(*gameObject->GetOwnerWorld()->GetPxScene());

	PxCapsuleControllerDesc capsuleDesc;
	capsuleDesc.height = mHeight;
	capsuleDesc.radius = mRadius;
	capsuleDesc.position = PxExtendedVec3(0, 0, 0);
	capsuleDesc.material = GameManager::GetPhysicsManager()->GetDefaultMaterial();
	capsuleDesc.density = 10.f;
	capsuleDesc.contactOffset = mContactOffset;
	capsuleDesc.slopeLimit = mSlopeLimit;
	capsuleDesc.stepOffset = mStepOffset;
	mCapsuleController = static_cast<PxCapsuleController*>(ControllerManager->createController(capsuleDesc));

	Vector3 pos = gameObject->transform->position;
	mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));

	const auto& materials = GameManager::GetPhysicsManager()->GetMaterials();
	for (auto& material : materials)
	{
		mMaterials.push_back(material.first.c_str());
	}
}

PlayerController::~PlayerController()
{
	ControllerManager->release();
	mCapsuleController->release();
}

void PlayerController::Start()
{
}

void PlayerController::Tick()
{
}

void PlayerController::FixedUpdate()
{
}

void PlayerController::PreUpdate()
{
}

void PlayerController::Update()
{
	//입력에 따른 move 
	
	//mMoveDirection *= mMoveSpeed;
	//mMoveDirection.y -= mGravity * deltaTime;
	//mCapsuleController->move(mMoveDirection, 0.001, deltaTime, mCharacterControllerFilters);
}

void PlayerController::PostUpdate()
{
	gameObject->transform->UpdatePxTransform();

}

void PlayerController::PreRender()
{
	PxExtendedVec3 pos = mCapsuleController->getPosition();
	gameObject->transform->position = Vector3(pos.x, pos.y, pos.z);
}

void PlayerController::Render()
{
}

void PlayerController::Draw(Camera* _camera)
{
}

void PlayerController::PostRender()
{
}

void PlayerController::EditorUpdate()
{
}

void PlayerController::EditorRender()
{
}

json PlayerController::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "PlayerController";

	ret["height"] = mHeight;
	ret["radius"] = mRadius;
	ret["contactOffset"] = mContactOffset;
	ret["slopeLimit"] = mSlopeLimit;
	ret["stepOffset"] = mStepOffset;

	ret["moveSpeed"] = mMoveSpeed;
	ret["jumpSpeed"] = mJumpSpeed;
	ret["gravity"] = mGravity;
	return ret;
}

void PlayerController::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if(j.contains("height"))
		mHeight = j["height"].get<float>();
	if (j.contains("radius"))
		mRadius = j["radius"].get<float>();
	if (j.contains("contactOffset"))
		mContactOffset = j["contactOffset"].get<float>();
	if (j.contains("slopeLimit"))
		mSlopeLimit = j["slopeLimit"].get<float>();
	if (j.contains("stepOffset"))
		mStepOffset = j["stepOffset"].get<float>();

	if (j.contains("moveSpeed"))
		mMoveSpeed = j["moveSpeed"].get<float>();
	if (j.contains("jumpSpeed"))
		mJumpSpeed = j["jumpSpeed"].get<float>();
	if (j.contains("gravity"))
		mGravity = j["gravity"].get<float>();
}

void PlayerController::SetMaterial(std::string _name)
{
	PxMaterial* material = GameManager::GetPhysicsManager()->GetMaterial(_name);

	PxShape* pxShapes[30];
	int size = mCapsuleController->getActor()->getShapes(pxShapes, 30);
	for (int i = 0; i < size; i++)
	{
		pxShapes[i]->setMaterials(&material, 1);
	}
}

void PlayerController::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	ImGui::Separator();
	ImGui::Text("Capsule Controller"); ImGui::SameLine;
	ImGui::Separator();
	ImGui::Text("Height : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "Height").c_str(), &mHeight, 0.1f, 0.f, 10.f))
	{
		mCapsuleController->setHeight(mHeight);
	}
	ImGui::Text("Radius : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "Radius").c_str(), &mRadius, 0.1f, 0.f, 10.f))
	{
		mCapsuleController->setRadius(mRadius);
	}
	ImGui::Text("ContactOffset : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "ContactOffset").c_str(), &mContactOffset, 0.01f, 0.1f, 1.0f))
	{
		mCapsuleController->setContactOffset(mContactOffset);
	}
	ImGui::Text("SlopeLimit : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "SlopeLimit").c_str(), &mSlopeLimit, 0.005f, 0.01f, 1.0f))
	{
		mCapsuleController->setSlopeLimit(mSlopeLimit);
	}
	ImGui::Text("StepOffset : "); ImGui::SameLine;
	if (ImGui::DragFloat((uid + "StepOffset").c_str(), &mStepOffset, 0.01f, 0.f, 10.f))
	{
		mCapsuleController->setStepOffset(mStepOffset);
	}

	std::vector<const char*> ccharMaterial;
	for (auto& mat : mMaterials)
	{
		ccharMaterial.push_back(mat.c_str());
	}

	if (ImGui::Combo((uid + "Dynamic Items").c_str(), &mMaterialIdx, ccharMaterial.data(), static_cast<int>(ccharMaterial.size())))
	{
		SetMaterial(mMaterials[mMaterialIdx]);
	}


	ImGui::Separator();
	ImGui::Text("Movement"); ImGui::SameLine;
	ImGui::Separator();

	ImGui::Text("MoveSpeed : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "MoveSpeed").c_str(), &mMoveSpeed, 0.1f, 0.f, 10.f);

	ImGui::Text("JumpSpeed : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "JumpSpeed").c_str(), &mJumpSpeed, 0.1f, 0.f, 10.f);

	ImGui::Text("Gravity : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "Gravity").c_str(), &mGravity, 0.1f, 0.f, 20.f);

	ImGui::Text("Mateiral : "); ImGui::SameLine;
	

	ImGui::Separator();
}
