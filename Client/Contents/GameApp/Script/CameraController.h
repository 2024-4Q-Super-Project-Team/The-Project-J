#pragma once

class CameraController : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CameraController);
    void Start();
    void Update();
    void SetCameraDirection(const Vector3& direction);
    void ResetCameraDirection();
private:
    void UpdateDistance();
    void UpdateRotation();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;

public:
    void LookAt(const Vector3& targetDirection, float duration, Dotween::EasingEffect easingEffect);
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

    // dotween
    bool isLookingAt = false;
    float lookAtDuration = 0.0f;
    float lookAtElapsedTime = 0.0f;
    Vector3 startDirection;
    Vector3 endDirection;
    Dotween::EasingEffect easingEffect;

};
