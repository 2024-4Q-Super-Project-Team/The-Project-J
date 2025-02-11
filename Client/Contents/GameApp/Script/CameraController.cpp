#include "pch.h"
#include "CameraController.h"
#include "Contents/GameApp/Script/GameProgressManager.h"
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

    // 원래 카메라 방향 저장
    mOriginalCameraDirection = mCameraDirection.val;
}

void CameraController::Update()
{
    if (isLookingAt)
    {
        lookAtElapsedTime += Time::GetScaledDeltaTime();
        float t = lookAtElapsedTime / lookAtDuration;
        mCameraDirection.val = Vector3::Lerp(startDirection, endDirection, Dotween::EasingFunction[static_cast<unsigned int>(easingEffect)](t));

        if (lookAtElapsedTime >= lookAtDuration)
        {
            isLookingAt = false;
            lookAtElapsedTime = 0.0f;
            mCameraDirection.val = endDirection;
        }
    }

    if (isTweeningOffset)
    {
        offsetTweenElapsedTime += Time::GetScaledDeltaTime();
        float t = offsetTweenElapsedTime / offsetTweenDuration;
        offSet = Vector3::Lerp(startOffset, targetOffset, Dotween::EasingFunction[static_cast<unsigned int>(offsetEasingEffect)](t));

        if (offsetTweenElapsedTime >= offsetTweenDuration)
        {
            isTweeningOffset = false;
            offsetTweenElapsedTime = 0.0f;
            offSet = targetOffset;
        }
    }

    if (isTweeningMidpointOffset)
    {
        midpointOffsetTweenElapsedTime += Time::GetScaledDeltaTime();
        float t = midpointOffsetTweenElapsedTime / midpointOffsetTweenDuration;
        midpointOffset = Vector3::Lerp(startMidpointOffset, targetMidpointOffset, Dotween::EasingFunction[static_cast<unsigned int>(midpointOffsetEasingEffect)](t));

        if (midpointOffsetTweenElapsedTime >= midpointOffsetTweenDuration)
        {
            isTweeningMidpointOffset = false;
            midpointOffsetTweenElapsedTime = 0.0f;
            midpointOffset = targetMidpointOffset;
        }
    }

    PlayerScript* Player1 = GameProgressManager::GetPlayerInfo(0);
    PlayerScript* Player2 = GameProgressManager::GetPlayerInfo(1);
    Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
    Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

    // 플레이어 간의 거리 벡터 계산
    Vector3 Between = Player1WorldPos - Player2WorldPos;
    float playerDistance = Between.Length();

    // 플레이어 거리를 카메라 거리로 매핑
    const float playerMinDist = 200.0f;
    const float playerMaxDist = 1000.0f;
    float distanceRatio = (playerDistance - playerMinDist) / (playerMaxDist - playerMinDist);
    distanceRatio = std::clamp(distanceRatio, 0.0f, 1.0f);

    float targetCameraDistance = mMinCameraDistance.val + (mMaxCameraDistance.val - mMinCameraDistance.val) * distanceRatio;

    // 현재 거리와 목표 거리가 일정 이상 차이나면 부드러운 전환 시작
    float threshold = 20.0f;
    if (abs(mCameraDistance.val - targetCameraDistance) > threshold)
    {
        gameObject->transform->ZoomTo(&mCameraDistance.val, mCameraDistance.val, targetCameraDistance, 1.0f);
    }

    // 중간 지점 계산
    Vector3 MidPoint = (Player1WorldPos + Player2WorldPos) * 0.5f;
    MidPoint += midpointOffset;

    // mCameraDirection을 정규화 (원본 변형 방지)
    Vector3 CameraDirection = mCameraDirection.val;
    CameraDirection.Normalize();

    // 카메라 위치 설정 (중간 지점에서 방향 벡터 * 거리 만큼 이동)
    Vector3 CameraPosition = MidPoint + (CameraDirection * mCameraDistance.val);
    CameraPosition += offSet;

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

void CameraController::SetCameraDirection(const Vector3& direction)
{
    mCameraDirection.val = direction;
}

void CameraController::ResetCameraDirection()
{
    mCameraDirection.val = mOriginalCameraDirection;
}

void CameraController::SetOffset(const Vector3& offset)
{
    this->offSet = offset;
}

void CameraController::SetMidpointOffset(const Vector3& offset)
{
    this->midpointOffset = offset;

}

void CameraController::LookAt(const Vector3& targetDirection, float duration, Dotween::EasingEffect easingEffect)
{
    if (isLookingAt) return;

    isLookingAt = true;
    lookAtDuration = duration;
    lookAtElapsedTime = 0.0f;
    this->easingEffect = easingEffect;

    startDirection = mCameraDirection.val;
    endDirection = targetDirection;
}


json CameraController::Serialize()
{
	json ret = MonoBehaviour::Serialize();

    ret["id"] = GetId();
    ret["name"] = "CameraController";
    ret["mindistance"] = mMinCameraDistance.val;
    ret["maxdistance"] = mMaxCameraDistance.val;

	return ret;
}

void CameraController::TweenOffset(const Vector3& targetOffset, float duration, Dotween::EasingEffect easingEffect)
{
    if (isTweeningOffset) return;

    isTweeningOffset = true;
    offsetTweenDuration = duration;
    offsetTweenElapsedTime = 0.0f;
    this->offsetEasingEffect = easingEffect;

    startOffset = offSet;
    this->targetOffset = targetOffset;
}

void CameraController::TweenMidpointOffset(const Vector3& targetOffset, float duration, Dotween::EasingEffect easingEffect)
{
    if (isTweeningMidpointOffset) return;

    isTweeningMidpointOffset = true;
    midpointOffsetTweenDuration = duration;
    midpointOffsetTweenElapsedTime = 0.0f;
    this->midpointOffsetEasingEffect = easingEffect;

    startMidpointOffset = midpointOffset;
    this->targetMidpointOffset = targetOffset;
}

void CameraController::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

    if (j.contains("mindistance"))
    {
        mMinCameraDistance.val = j["mindistance"].get<float>();
    }

    if (j.contains("maxdistance"))
    {
        mMaxCameraDistance.val = j["maxdistance"].get<float>();
    }

}