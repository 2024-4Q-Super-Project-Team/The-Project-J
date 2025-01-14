#pragma once



class CameraController : public MonoBehaviour
{
public:
	using MonoBehaviour::MonoBehaviour;
	~CameraController();
	void Start();
	void Update();

public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:

	float mMoveSpeed;
	Transform* mTr;
	Camera* mCamera;
    
    float sensitivity = 0.005f;
    Radian maxYAngle = 1.0f;
    Radian CurrentAngles[3];

    SerializeField(Vector3, vec);

public:
    enum AxisType
    {
        Yaw,
        Pitch,
        Roll
    };
};
