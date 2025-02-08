#pragma once


class BoxScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BoxScript);

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(Rigidbody* box, Rigidbody* player) override;
	virtual void OnCollisionExit(Rigidbody* box, Rigidbody* player) override;

private:
	void MoveBox(Vector3 displacement);
private:
	BoxCollider* mCollider;
	Rigidbody* mRigid;
	float mMaxDistance = 1.5f;
	PxVec3 mPxRayOrigin;
	PxVec3 mPxRayDirection;
	PxRaycastBuffer mHitBuffer;

	float mMoveSpeed = 200.f;
	float mGravitySpeed = 0.1f;
	SerializeField(bool, mGravityOn, false);
	SerializeField(Vector3, displacement, Vector3(0,0,0));
};
