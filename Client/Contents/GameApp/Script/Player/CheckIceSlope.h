#pragma once

class BurnObjectScript;

class CheckIceSlope :  public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CheckIceSlope);

    virtual void Start() override;
public:
    virtual void _CALLBACK OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination);
    virtual void _CALLBACK OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination);
    virtual void _CALLBACK OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination);


public:
    SerializeField(float, NotSlidingTime, 0.3f);
    SerializeField(float, SlidingTime, 3.0f);
private:
    BurnObjectScript* mBurnScript;
    PlayerController* mController;

    float mNotSlidingElapsedTime = 0.f;
    float mSlidingElapsedTime = 0.f;

    bool mIsSliding = false;

    float mSlideGravity = 2.1f;
    float mInitialGravity = 0.5f;
};

