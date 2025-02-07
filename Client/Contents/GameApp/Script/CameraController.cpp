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

    mCameraDistance.val = 700.0f;
	mCameraDirection.val = Vector3(0.0f, 0.05f, -0.035f);
    mZoomSpeed = 5.0f;
    mLerpSpeed = 3.0f;
}

void CameraController::Update()
{
    PlayerScript* Player1 = PlayerManager::GetPlayerInfo(0);
    PlayerScript* Player2 = PlayerManager::GetPlayerInfo(1);
    Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
    Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

    // 플레이어 간의 거리 벡터 계산
    Vector3 Between = Player1WorldPos - Player2WorldPos;
    float playerDistance = Between.Length();

    // 플레이어 거리(200~1000)를 카메라 거리(600~1000)로 매핑
    const float playerMinDist = 200.0f;
    const float playerMaxDist = 1000.0f;
    float distanceRatio = (playerDistance - playerMinDist) / (playerMaxDist - playerMinDist);
    distanceRatio = std::clamp(distanceRatio, 0.0f, 1.0f);

    float targetCameraDistance = mMinCameraDistance + (mMaxCameraDistance - mMinCameraDistance) * distanceRatio;

    // 현재 거리와 목표 거리가 일정 이상 차이나면 부드러운 전환 시작
    float threshold = 20.0f;
    if (abs(mCameraDistance.val - targetCameraDistance) > threshold)
    {
        gameObject->transform->ZoomTo(&mCameraDistance.val, mCameraDistance.val, targetCameraDistance, 1.0f);
    }

    // 중간 지점 계산
    Vector3 MidPoint = (Player1WorldPos + Player2WorldPos) * 0.5f;

    // mCameraDirection을 정규화 (원본 변형 방지)
    Vector3 CameraDirection = mCameraDirection.val;
    CameraDirection.Normalize();

    // 카메라 위치 설정 (중간 지점에서 방향 벡터 * 거리 만큼 이동)
    Vector3 CameraPosition = MidPoint + (CameraDirection * mCameraDistance.val);
    gameObject->transform->position = CameraPosition;

    // 카메라의 Forward 및 Right 벡터 계산
    Vector3 ForwardVector = MidPoint - CameraPosition;
    ForwardVector.Normalize();

    Vector3 RightVector = Vector3(0.0f, 1.0f, 0.0f).Cross(ForwardVector); // 카메라의 오른쪽 벡터
    RightVector.Normalize();

    Vector3 UpVector = ForwardVector.Cross(RightVector); // 카메라의 Up 벡터

    // 플레이어 간 거리 벡터를 카메라의 로컬 좌표계로 변환
    float distanceHorizontal = fabs(Between.Dot(RightVector));  // 가로 거리
    float distanceDepth = fabs(Between.Dot(ForwardVector));     // 세로 거리

    // 카메라 거리 조정 (화면 크기 대비)
    Vector2 ScreenSize = { mCamera->GetViewport()->mViewport.Width, mCamera->GetViewport()->mViewport.Height };
    float ScreenThresholdX = ScreenSize.x * 0.5f; // 가로 크기 한계
    float ScreenThresholdZ = ScreenSize.y * 0.5f; // 세로 크기 한계

    // LookAt 호출 (업 벡터 적용)
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
