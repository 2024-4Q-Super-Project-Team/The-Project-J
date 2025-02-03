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
	mCapsuleDesc.stepOffset = 0.f;
	mCapsuleDesc.scaleCoeff = 1.0f;
	//mCapsuleDesc.behaviorCallback = mIceBehavior;
	mCapsuleDesc.maxJumpHeight = 20.f;
	//mCapsuleDesc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
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

	//for Trigger/Contact Event
	//Inner Rigidbody
	mRigid = new Rigidbody(gameObject);
	mCapsuleController->getActor()->userData = mRigid;
	mRigid->SetPxActor(mCapsuleController->getActor());

	//Inner Shapes
	PxShape* shapes[10];
	PxU32 shapeSize = mCapsuleController->getActor()->getShapes(shapes, 10);
	for (int i = 0; i < shapeSize; i++) {
		Collider* col = new Collider(gameObject);
		shapes[i]->userData = col;
		mColliders.push_back(col);
	}
}

PlayerController::~PlayerController()
{
	mCapsuleController->release();

	mRigid->mRigidActor = nullptr;
	SAFE_DELETE(mRigid);
	for (Collider* col : mColliders)
	{
		col->mShape = nullptr;
		SAFE_DELETE(col);
	}
	SAFE_DELETE(mIceBehavior);
}

void PlayerController::Start()
{
	Vector3 pos = gameObject->transform->position;
	mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));

	SetMaterial(mMaterials[mMaterialIdx]);

	mCapsuleController->setUserData(this);

	mCapsuleController->setHeight(mHeight);
	mCapsuleController->setRadius(mRadius);
	mCapsuleController->setContactOffset(0.f);
	mCapsuleController->setSlopeLimit(mSlopeLimit);
	mCapsuleController->setStepOffset(0.f);
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
	t = Time::GetScaledDeltaTime();

	mDisplacement = PxVec3(0, 0, 0);

	CheckOnGround();
	KeyboardMove();
	PadMove();
	GravityUpdate();
	JumpUpdate();
	

	PxControllerCollisionFlags flags = mCapsuleController->move(mDisplacement, 0.001f, t, mCharacterControllerFilters);


	if (flags & PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		int a = 0;
	}
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

void PlayerController::KeyboardMove()
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
	mDisplacement += moveDirection * mMoveSpeed * t;

}

void PlayerController::PadMove()
{
	
}

void PlayerController::CheckOnGround()
{
	mIsOnGround = false;

	//바닥 감지 
	Vector3 objPos = gameObject->transform->position;
	PxVec3 pxRayOrigin(objPos.x, objPos.y - mCapsuleController->getHeight() / 2.f - mCapsuleController->getRadius(), objPos.z);
	PxVec3 pxRayDirection(0, -1, 0);
	float distance = 0.5;

	PxRaycastBuffer hitBuffer;
	if (GameManager::GetCurrentWorld()->GetPxScene()
		->raycast(pxRayOrigin, pxRayDirection, distance, hitBuffer))
	{
		const PxRaycastHit& hit = hitBuffer.block;
		if (hit.distance < distance)
		{
			mIsOnGround = true;
		}
	}
}

void PlayerController::GravityUpdate()
{
	//중력 
	//if (mIsOnGround == false)
	mDisplacement.y -= mGravity * t;
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

	ret["jump duration"] = mJumpDuration;

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

	if (j.contains("jump duration"))
		mJumpDuration = j["jump duration"].get<float>();
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

	ImGui::Text("Jump Duration : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "Jump Duration").c_str(), &mJumpDuration, 0.01f, 0.01f, 20.f);


	ImGui::Text("Gravity : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "Gravity").c_str(), &mGravity, 0.1f, 0.f, 20.f);

	ImGui::Text("Displacement : %.3f, %.3f, %.3f", mDisplacement.x, mDisplacement.y, mDisplacement.z); ImGui::SameLine;
	

	ImGui::Separator();
}
void PlayerController::JumpUpdate()
{
	mJumpInputElapsedTime = 0.f;

	//점프
	if (mJumpState == eJumpState::None && Input::IsKeyHold(Key::keyMap[mStrKeys[mJumpKeyIdx]]))
	{
		mJumpState = eJumpState::InitJump;
		mJumpInitElapsedTime += t;
	}
	if (mJumpState == eJumpState::InitJump && Input::IsKeyUp(Key::keyMap[mStrKeys[mJumpKeyIdx]]))
	{
		//mJumpInitElapsedTime에 따라 mJumpSpeed를 조절. 
		//TODO 

		mJumpInitElapsedTime = 0.f;
		mJumpState = eJumpState::Jumping;
	}

	if (mJumpState == eJumpState::Jumping)
	{
		mJumpElapsedTime += t;
		mJumpInputElapsedTime += t;
		float t = mJumpElapsedTime / mJumpDuration;
		mDisplacement.y = mJumpSpeed * (1 - t);

		if (t >= 1.0f) {
			mJumpState = eJumpState::None;
			mDisplacement.y = 0.f;
			mJumpElapsedTime = 0.f;
			mJumpInputElapsedTime = 0.f;
		}
	}
}
void PlayerController::StartJump()
{
	mJumpInitElapsedTime = 0.f;
	mJumpState = eJumpState::Jumping;
}