#include "pch.h"
#include "PlayerController.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
#include "World/WorldManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Component/Controller/PlayerBehaviorCallback.h"
#include "Component/Controller/ControllerEventCallback.h"


PlayerController::PlayerController(Object* _owner) :Component(_owner)
{
	SetEID("PlayerController");
	mType = eComponentType::CONTROLLER;

	mBehaviorCallback = new DynamicBehaviorCallback;
	SetSlopeMode(mSlopeMode);

	mEventCallback = new ControllerEventCallback;

	mCapsuleDesc.height = mHeight;
	mCapsuleDesc.radius = mRadius;
	mCapsuleDesc.position = PxExtendedVec3(0, 0, 0);
	mCapsuleDesc.material = GameManager::GetPhysicsManager()->GetDefaultMaterial();
	mCapsuleDesc.density = 10.f;
	mCapsuleDesc.contactOffset = mContactOffset;
	mCapsuleDesc.slopeLimit = mSlopeLimit;
	mCapsuleDesc.stepOffset = 0.f;
	mCapsuleDesc.scaleCoeff = 1.0f;
	mCapsuleDesc.density = 10.f;
	mCapsuleDesc.behaviorCallback = mBehaviorCallback;
	mCapsuleDesc.reportCallback = mEventCallback;
	mCapsuleDesc.maxJumpHeight = 20.f;
	PxControllerManager* controllerManager = gameObject->GetOwnerWorld()->GetControllerManager();
	mCapsuleController = static_cast<PxCapsuleController*>(controllerManager->createController(mCapsuleDesc));

	mCapsuleController->setUserData(this);


	const auto& materials = GameManager::GetPhysicsManager()->GetMaterials();
	for (auto& material : materials)
	{
		mMaterials.push_back(material.first.c_str());
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
	SAFE_DELETE(mBehaviorCallback);
	SAFE_DELETE(mEventCallback);

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

	PxControllerCollisionFlags flags = mCapsuleController->move(mDisplacement, 0.001f, t, mCharacterControllerFilters);
	mIsOnGround = flags & PxControllerCollisionFlag::eCOLLISION_DOWN;

	GravityUpdate();


}

void PlayerController::PostUpdate()
{
	gameObject->transform->UpdatePxTransform();

}

void PlayerController::PreRender()
{
	PxExtendedVec3 pos = mCapsuleController->getPosition();
	gameObject->transform->position = Vector3(pos.x, pos.y, pos.z);

	CheckNowColliding();
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

void PlayerController::Move(Vector3 _displacement)
{
	mDisplacement = PxVec3(_displacement.x, _displacement.y, _displacement.z);
}


void PlayerController::SetSlopeMode(SlopeMode _mode)
{
	mSlopeMode = _mode; 
	if(_mode == SlopeMode::Ride)
		mBehaviorCallback->SetFlags(PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT);
	else if (_mode == SlopeMode::Slide)
		mBehaviorCallback->SetFlags(PxControllerBehaviorFlag::eCCT_SLIDE);
}

void PlayerController::GravityUpdate()
{
	//중력 
	if (mIsOnGround == false)
	{
		mGravityElapsedTime += t;
		float gt = mGravityElapsedTime;
		float s = gt + (1 / 2) * mGravity * gt * gt;
	}
	else
		mGravityElapsedTime = 0.f;
}

void PlayerController::CheckNowColliding()
{
	for (auto it = mActorsColliding.begin(); it != mActorsColliding.end();)
	{
		if ((*it).second == false) //이번에 충돌하지 않았음 
		{
			mEventCallback->EraseCollideActor(mCapsuleController, (*it).first);

			Rigidbody* collidingRigidbody = static_cast<Rigidbody*>((*it).first->userData);

			Object* triggerObject = static_cast<Object*>(mRigid->gameObject);
			Object* otherObject = static_cast<Object*>(collidingRigidbody->gameObject);

			auto triggerScripts = triggerObject->GetComponents<MonoBehaviour>();
			auto otherScripts = otherObject->GetComponents<MonoBehaviour>();

			for (auto script : triggerScripts)
				script->OnCollisionExit(mRigid, collidingRigidbody);
			for (auto script : otherScripts)
				script->OnCollisionExit(collidingRigidbody, mRigid);

			it = mActorsColliding.erase(it);
		}
		else
		{
			(*it).second = false;
			++it;
		}
			
	}
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

	ret["gravity"] = mGravity;

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

	if (j.contains("gravity"))
		mGravity = j["gravity"].get<float>();

	if (j.contains("slope mode"))
	{
		mSlopeMode = (SlopeMode)j["slope mode"].get<int>();
		mSlopeModeIdx = (int)mSlopeMode;
	}

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



	ImGui::Text("Gravity : "); ImGui::SameLine;
	ImGui::DragFloat((uid + "Gravity").c_str(), &mGravity, 0.1f, 0.f, 20.f);

	ImGui::Text("Displacement : %.3f, %.3f, %.3f", mDisplacement.x, mDisplacement.y, mDisplacement.z); ImGui::SameLine;
	
	ImGui::Separator();
	std::vector<const char*> ccharSlopeMode = {"Ride", "Slide"};
	ImGui::Text(u8"경사면 모드"); ImGui::SameLine;
	if (ImGui::Combo((uid + "SlopeMode").c_str(), &mSlopeModeIdx, ccharSlopeMode.data(), static_cast<int>(ccharSlopeMode.size())))
	{
		SetSlopeMode((SlopeMode)mSlopeModeIdx);
	}

	ImGui::Separator();
}
