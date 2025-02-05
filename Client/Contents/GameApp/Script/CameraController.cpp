#include "pch.h"
#include "CameraController.h"
#include "Manager/PlayerManager.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void CameraController::Start()
{
	mCamera = gameObject->GetComponent<Camera>();
	gameObject->AddComponent<AudioListener>();

	CurrentAngles[Yaw] = Degree::ToRadian(180);
	CurrentAngles[Pitch] = 0;
	CurrentAngles[Roll] = 0;

    mCameraDistance.val = 1000.0f;
	mCameraDirection.val = Vector3(0.0f, 0.05f, -0.05f);
}

void CameraController::Update()
{
    PlayerScript* Player1 = PlayerManager::GetPlayerInfo(0);
    PlayerScript* Player2 = PlayerManager::GetPlayerInfo(1);
    Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
    Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

    // �÷��̾� ���� �Ÿ� ���� ���
    Vector3 Between = Player1WorldPos - Player2WorldPos;

    // �߰� ���� ���
    Vector3 MidPoint = (Player1WorldPos + Player2WorldPos) * 0.5f;

    // mCameraDirection�� ����ȭ (���� ���� ����)
    Vector3 CameraDirection = mCameraDirection.val;
    CameraDirection.Normalize();

    // ī�޶� ��ġ ���� (�߰� �������� ���� ���� * �Ÿ� ��ŭ �̵�)
    Vector3 CameraPosition = MidPoint + (CameraDirection * mCameraDistance.val);
    gameObject->transform->position = CameraPosition;

    // ī�޶��� Forward �� Right ���� ���
    Vector3 ForwardVector = MidPoint - CameraPosition;
    ForwardVector.Normalize();

    Vector3 RightVector = Vector3(0.0f, 1.0f, 0.0f).Cross(ForwardVector); // ī�޶��� ������ ����
    RightVector.Normalize();

    Vector3 UpVector = ForwardVector.Cross(RightVector); // ī�޶��� Up ����

    // �÷��̾� �� �Ÿ� ���͸� ī�޶��� ���� ��ǥ��� ��ȯ
    float distanceHorizontal = fabs(Between.Dot(RightVector));  // ���� �Ÿ�
    float distanceDepth = fabs(Between.Dot(ForwardVector));     // ���� �Ÿ�

    // ī�޶� �Ÿ� ���� (ȭ�� ũ�� ���)
    Vector2 ScreenSize = { mCamera->GetViewport()->mViewport.Width, mCamera->GetViewport()->mViewport.Height };
    float ScreenThresholdX = ScreenSize.x * 0.5f; // ���� ũ�� �Ѱ�
    float ScreenThresholdZ = ScreenSize.y * 0.5f; // ���� ũ�� �Ѱ�

    // LookAt ȣ�� (�� ���� ����)
    gameObject->transform->LookAt(MidPoint, UpVector);
}


json CameraController::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	return ret;
}

void CameraController::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

}
