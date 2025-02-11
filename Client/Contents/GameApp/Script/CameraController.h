#pragma once

class CameraController : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CameraController);
    void Start();
    void Update();
    void SetCameraDirection(const Vector3& direction);
    void ResetCameraDirection();
    void SetOffset(const Vector3& offset);
    void SetMidpointOffset(const Vector3& offset);
    void TweenOffset(const Vector3& targetOffset, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);
    void TweenMidpointOffset(const Vector3& targetOffset, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);

private:
    void UpdateDistance();
    void UpdateRotation();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;

public:
    void LookAt(const Vector3& targetDirection, float duration, Dotween::EasingEffect easingEffect);
    void LookAtTarget(const Vector3& targetPosition, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);
    void MoveTo(const Vector3& targetPosition, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);
    void MoveAndLookAt(const Vector3& targetPosition, const Vector3& lookAtPosition, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);

public:
    Vector3 mOriginalCameraDirection;
    SerializeField(FLOAT, mCameraDistance, 200.0f);
    SerializeField(Vector3, mCameraDirection, Vector3::One);
    SerializeField(FLOAT, mMinCameraDistance, 600.0f);
    SerializeField(FLOAT, mMaxCameraDistance, 1000.0f);
    FLOAT CurrentAngles[3];
    Camera* mCamera;
public:
    enum AxisType
    {
        Yaw,
        Pitch,
        Roll
    };
private:
    float mZoomSpeed;
    float mLerpSpeed;
    Vector3 offSet = Vector3::Zero;
    Vector3 midpointOffset = Vector3::Zero;

    // dotween
    // LookAt
    bool isLookingAt = false;
    float lookAtDuration = 0.0f;
    float lookAtElapsedTime = 0.0f;
    Vector3 startDirection;
    Vector3 endDirection;
    Dotween::EasingEffect easingEffect;

    // Moveto
    bool isMoving = false;
    float moveDuration = 0.0f;
    float moveElapsedTime = 0.0f;
    Vector3 startPosition;
    Vector3 endPosition;
    Dotween::EasingEffect moveEasingEffect;

    // Offset 관련
    bool isTweeningOffset = false;
    float offsetTweenDuration = 0.0f;
    float offsetTweenElapsedTime = 0.0f;
    Vector3 startOffset;
    Vector3 targetOffset;
    Dotween::EasingEffect offsetEasingEffect;

    // MidpointOffset 관련
    bool isTweeningMidpointOffset = false;
    float midpointOffsetTweenDuration = 0.0f;
    float midpointOffsetTweenElapsedTime = 0.0f;
    Vector3 startMidpointOffset;
    Vector3 targetMidpointOffset;
    Dotween::EasingEffect midpointOffsetEasingEffect;

    // 업데이트 중지 플래그
    bool isUpdateEnabled = true;
    bool isMoveComplete = false;
    bool isLookAtComplete = false;




};
