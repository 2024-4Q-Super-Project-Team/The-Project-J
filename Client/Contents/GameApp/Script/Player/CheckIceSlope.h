#pragma once

class BurnObjectScript;

class CheckIceSlope :  public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CheckIceSlope);
public:
    virtual void Start() override;
    virtual void Update() override;

    virtual void _CALLBACK OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination);
    virtual void _CALLBACK OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination);
    virtual void _CALLBACK OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination);

private:
    BurnObjectScript* mBurnScript;
    PlayerController* mController;
};

