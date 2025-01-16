#include "pch.h"
#include "PlayerController.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"

void PlayerController::Start()
{
	if (ControllerManager == nullptr)
		ControllerManager = PxCreateControllerManager(*GameManager::GetCurrentWorld()->GetPxScene());

	PxCapsuleControllerDesc capsuleDesc;
	capsuleDesc.height = 4;
	capsuleDesc.radius = 2;
	capsuleDesc.position = PxExtendedVec3(0, 0, 0);
	capsuleDesc.material = GameManager::GetPhysicsManager()->GetDefaultMaterial();
	capsuleDesc.density = 100.0f;
	capsuleDesc.contactOffset = 0.05f;
	capsuleDesc.slopeLimit = 0.2f;
	capsuleDesc.stepOffset = 0.75f;

	mCapsuleController = static_cast<PxCapsuleController*>(ControllerManager->createController(capsuleDesc));


	Vector3 pos = gameObject->transform->position;
	mCapsuleController->setPosition(PxExtendedVec3(pos.x, pos.y, pos.z));
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
	
	//mMoveDirection *= mSpeed;
	//mMoveDirection.y -= mGravity * deltaTime;
	//mCapsuleController->move(mMoveDirection, 0.001, deltaTime, mCharacterControllerFilters);
}

void PlayerController::PostUpdate()
{
	PxExtendedVec3 pos = mCapsuleController->getPosition();
	gameObject->transform->position = Vector3(pos.x, pos.y, pos.z);
}

void PlayerController::PreRender()
{
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
