#pragma once

class CameraController : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CameraController);
    void Start();
    void Update();
public:
    virtual json Serialize() override;
public:
    SerializeField(FLOAT, mMoveSpeed, 100.0f);
    SerializeField(FLOAT, sensitivity, 5.0f);
    SerializeField(FLOAT, maxYAngle, 3.0f);
    FLOAT CurrentAngles[3];
    FLOAT CurrentSubCameraAngles[3];
    Transform* mTr;
    Camera* mCamera;
    Camera* mSubCamera;
public:
    enum AxisType
    {
        Yaw,
        Pitch,
        Roll
    };
};
