#pragma once

class CameraController : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CameraController);
    void Start();
    void Update();
private:
    void UpdateDistance();
    void UpdateRotation();
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:
    SerializeField(FLOAT, mCameraDistance, 200.0f);
    SerializeField(Vector3, mCameraDirection, Vector3::One);
    FLOAT CurrentAngles[3];
    Camera* mCamera;
public:
    enum AxisType
    {
        Yaw,
        Pitch,
        Roll
    };
};
