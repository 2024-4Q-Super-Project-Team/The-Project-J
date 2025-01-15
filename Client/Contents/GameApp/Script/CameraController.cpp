#include "pch.h"
#include "CameraController.h"

CameraController::~CameraController()
{
}

void CameraController::Start()
{
	// 스카이 박스
	auto pSkyBox = ResourceManager::AddResource<SkyBox>(L"Default_SkyBox");
	pSkyBox->SetEnvironmentTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapEnvHDR.dds"));
	pSkyBox->SetDiffuseTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapDiffuseHDR.dds"));
	pSkyBox->SetSpecularture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapSpecularHDR.dds"));
	pSkyBox->SetLookUpTableTexture(ResourceManager::AddResource<Texture2D>(L"resource/texture/Skybox/DefaultSkyIBLMapBrdf.dds"));

	mCamera = FindObject(L"Main_Camera", L"Default")->GetComponent<Camera>();
	mCamera->SetSkyBox(pSkyBox);
	mCamera->gameObject->AddComponent<AudioListener>();

	mTr = mCamera->gameObject->transform;
	mMoveSpeed = 100.0f;

	mTr->position = Vector3(0, 100, -100);

	CurrentAngles[Yaw] = Degree::ToRadian(180);
	CurrentAngles[Pitch] = 0;
	CurrentAngles[Roll] = 0;

	CurrentSubCameraAngles[Yaw] = Degree::ToRadian(180);
	CurrentSubCameraAngles[Pitch] = 0;
	CurrentSubCameraAngles[Roll] = 0;

	auto SubCamera = CreateObject(L"Sub_Camera", L"Default");
	SubCamera->transform->position = Vector3(0, 100, -100);
	mSubCamera = SubCamera->AddComponent<Camera>();
	mSubCamera->SetCameraSize(Vector2(0.3f, 0.3f));
	mSubCamera->SetSkyBox(pSkyBox);
}

void CameraController::Update()
{
	Vector3 InputVector = Vector3::Zero;
	if (Input::IsMouseHold(Mouse::RIGHT) || Input::IsMouseHold(Mouse::MID))
	{
		Input::ShowMouseCursor(false);
		// 각도를 라디안으로 계산 (Pitch, Yaw)
		Radian deltaAngleX;
		Radian deltaAngleY;
		deltaAngleX = Input::GetDeltaMousePos().x * sensitivity;
		deltaAngleY = -Input::GetDeltaMousePos().y * sensitivity;
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			CurrentSubCameraAngles[Yaw] += deltaAngleX;
			CurrentSubCameraAngles[Pitch] += deltaAngleY;
			CurrentSubCameraAngles[Pitch] = Clamp(CurrentSubCameraAngles[Pitch], -maxYAngle, maxYAngle);
		}
		else
		{
			CurrentAngles[Yaw] += deltaAngleX;
			CurrentAngles[Pitch] += deltaAngleY;
			CurrentAngles[Pitch] = Clamp(CurrentAngles[Pitch], -maxYAngle, maxYAngle);
		}
	}
	else
	{
		Input::ShowMouseCursor(true);
	}
	if (Input::IsKeyHold('Q'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed * mSubCamera->gameObject->transform->Up() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('E'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed * mSubCamera->gameObject->transform->Up() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed * mTr->Up() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('W'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('S'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed * mTr->Forward() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('A'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += mMoveSpeed * mSubCamera->gameObject->transform->Right() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position += mMoveSpeed * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::IsKeyHold('D'))
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position -= mMoveSpeed * mSubCamera->gameObject->transform->Right() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
			mTr->position -= mMoveSpeed * mTr->Right() * Time::GetScaledDeltaTime();
	}
	if (Input::GetWheelDeltaPos() != 0)
	{
		if (Input::IsKeyHold(Key::LCONTROL))
		{
			mSubCamera->gameObject->transform->position += Input::GetWheelDeltaPos() * mSubCamera->gameObject->transform->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
		else
		{
			mTr->position += Input::GetWheelDeltaPos() * mTr->Forward() * Time::GetScaledDeltaTime() * 10.0f;
		}
	}

	mTr->rotation
		= DirectX::XMQuaternionRotationRollPitchYaw(
			CurrentAngles[Pitch],
			CurrentAngles[Yaw],
			CurrentAngles[Roll]
		);

	mSubCamera->gameObject->transform->rotation
		= DirectX::XMQuaternionRotationRollPitchYaw(
			CurrentSubCameraAngles[Pitch],
			CurrentSubCameraAngles[Yaw],
			CurrentSubCameraAngles[Roll]
		);
}

json CameraController::Serialize()
{
	return json();
}
