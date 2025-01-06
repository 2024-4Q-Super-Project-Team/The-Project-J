#pragma once



class CameraController : public MonoBehaviour
{
public:
	using MonoBehaviour::MonoBehaviour;
	~CameraController();
	void Start();
	void Update();

	float mMoveSpeed;
	Transform* mTr;
	Camera* mCamera;

    float sensitivity = 0.005f;
    Radian maxYAngle = 1.0f;
    Radian CurrentAngles[3];
public:
    enum AxisType
    {
        Yaw,
        Pitch,
        Roll
    };
};
