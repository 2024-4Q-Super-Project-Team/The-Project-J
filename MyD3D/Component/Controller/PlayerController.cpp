#include "pch.h"
#include "PlayerController.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
#include "World/WorldManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Component/Controller/PlayerBehaviorCallback.h"


PlayerController::PlayerController(Object* _owner) :Component(_owner)
{
	SetEID("PlayerController");
	mType = eComponentType::CONTROLLER;

	mIceBehavior = new PlayerBehaviorCallback;

	mCapsuleDesc.height = mHeight;
	mCapsuleDesc.radius = mRadius;
	mCapsuleDesc.position = PxExtendedVec3(0, 0, 0);
	mCapsuleDesc.material = GameManager::GetPhysicsManager()->GetDefaultMaterial();
	mCapsuleDesc.density = 10.f;
	mCapsuleDesc.contactOffset = mContactOffset;
	mCapsuleDesc.slopeLimit = mSlopeLimit;
	mCapsuleDesc.stepOffset = mStepOffset;
	mCapsuleDesc.behaviorCallback = mIceBehavior;
	mCapsuleDesc.maxJumpHeight = 100.f;
	mCapsuleDesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	PxControllerManager* controllerManager = gameObject->GetOwnerWorld()->GetControllerManager();
	mCapsuleController = static_cast<PxCapsuleController*>(controllerManager->createController(mCapsuleDesc));

	const auto& materials = GameManager::GetPhysicsManager()->GetMaterials();
	for (auto& material : materials)
	{
		mMaterials.push_back(material.first.c_str());
	}

	for (auto key : Key::keyMap)
	{
		mStrKeys.push_back(key.first);
	}
}

PlayerController::~PlayerController()
{
	mCapsuleController->release();
	SAFE_DELETE(mIceBehavior);
}

void PlayerController::Start()
{
	Vector3 pos = gameObject->transform->position;
	mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));

	SetMaterial(mMaterials[mMaterialIdx]);
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
	PxVec3 moveDirection = PxVec3(0.f, 0.f, 0.f);
	//입력에 따른 move 

	if (Input::IsKeyHold(Key::keyMap[mStrKeys[mForwardKeyIdx]]))
	{
		moveDirection += PxVec3(0, 0, 1);
	}
	if (Input::IsKeyHold(Key::keyMap[mStrKeys[mBackwardKeyIdx]]))
	{
		moveDirection += PxVec3(0, 0, -1);
	}
	if (Input::IsKeyHold(Key::keyMap[mStrKeys[mLeftKeyIdx]]))
	{
		moveDirection += PxVec3(-1, 0, 0);
	}
	if (Input::IsKeyHold(Key::keyMap[mStrKeys[mRightKeyIdx]]))
	{
		moveDirection += PxVec3(1, 0, 0);
	}

	if (!moveDirection.isZero())
		moveDirection.normalize();

	//이동
	mMoveVelocity = moveDirection * mMoveSpeed * Time::GetScaledDeltaTime();


	//점프
	if (!mIsJumping && Input::IsKeyDown(Key::keyMap[mStrKeys[mJumpKeyIdx]]))
	{
		mIsJumping = true;
	}

	if (mIsJumping)
	{
		mJumpElapsedTime += Time::GetScaledDeltaTime();
		float t = mJumpElapsedTime / mJumpDuration;
		mMoveVelocity.y = mJumpSpeed * (1-t) ;

		if (t >= 1.0f) {
			mIsJumping = false;
			mMoveVelocity.y = 0.f;
			mJumpElapsedTime = 0.f;
		}
	}

	//중력 
	mMoveVelocity.y -= mGravity * Time::GetScaledDeltaTime();

	mCapsuleController->move(mMoveVelocity, 0.01f, Time::GetScaledDeltaTime(), mCharacterControllerFilters);


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

void _CALLBACK PlayerController::OnEnable()
{
	if (!mCapsuleController)
	{
		PxControllerManager* controllerManager = gameObject->GetOwnerWorld()->GetControllerManager();
		mCapsuleController = static_cast<PxCapsuleController*>(controllerManager->createController(mCapsuleDesc));
		Vector3 pos = gameObject->transform->position;
		mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
	}
	return void _CALLBACK();
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

	ret["forward"] = mForwardKeyIdx;
	ret["backward"] = mBackwardKeyIdx;
	ret["left"] = mLeftKeyIdx;
	ret["right"] = mRightKeyIdx;
	ret["jump"] = mJumpKeyIdx;

	ret["material"] = mMaterialIdx;

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

	if (j.contains("forward"))
		mForwardKeyIdx = j["forward"].get<int>();
	if (j.contains("backward"))
		mBackwardKeyIdx = j["backward"].get<int>();
	if (j.contains("left"))
		mLeftKeyIdx = j["left"].get<int>();
	if (j.contains("right"))
		mRightKeyIdx = j["right"].get<int>();
	if (j.contains("jump"))
		mJumpKeyIdx = j["jump"].get<int>();

	if (j.contains("material"))
		mMaterialIdx = j["material"].get<int>();
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

	//////////////////////////Key Setting//////////////////////

	std::vector<const char*> ccharKeys;
	for (auto& key : mStrKeys)
	{
		ccharKeys.push_back(key.c_str());
	}
	ImGui::Text("Forward : "); ImGui::SameLine;
	ImGui::Combo((uid + "forwardkey").c_str(), &mForwardKeyIdx, ccharKeys.data(), static_cast<int>(ccharKeys.size()));
	ImGui::Text("Backwrad : "); ImGui::SameLine; 
	ImGui::Combo((uid + "backwardkey").c_str(), &mBackwardKeyIdx, ccharKeys.data(), static_cast<int>(ccharKeys.size()));
	ImGui::Text("Left : "); ImGui::SameLine; 
	ImGui::Combo((uid + "leftkey").c_str(), &mLeftKeyIdx, ccharKeys.data(), static_cast<int>(ccharKeys.size()));
	ImGui::Text("Right : "); ImGui::SameLine; 
	ImGui::Combo((uid + "rightkey").c_str(), &mRightKeyIdx, ccharKeys.data(), static_cast<int>(ccharKeys.size()));
	ImGui::Text("Jump : "); ImGui::SameLine; 
	ImGui::Combo((uid + "jumpkey").c_str(), &mJumpKeyIdx, ccharKeys.data(), static_cast<int>(ccharKeys.size()));
	ImGui::Separator();
	////////////////////////////////////////////////////////////

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
	if (ImGui::DragFloat((uid + "ContactOffset").c_str(), &mContactOffset, 0.01f, 0.1f, 100.0f))
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

void PlayerController::StartJump()
{
	mIsJumping = true;
	mJumpElapsedTime = 0.f;
}