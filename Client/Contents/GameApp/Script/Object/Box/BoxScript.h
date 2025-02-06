#pragma once

class BoxScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BoxScript);

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	BoxCollider* mCollider;
	Rigidbody* mRigid;
	float mMaxDistance = .5f;
	PxVec3 mPxRayOrigin;
	PxVec3 mPxRayDirection;
	PxRaycastBuffer mHitBuffer;
};

