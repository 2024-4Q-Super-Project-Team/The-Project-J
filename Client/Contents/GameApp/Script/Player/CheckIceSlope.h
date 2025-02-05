#pragma once

class BurnObjectScript;

class CheckIceSlope :  public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(CheckIceSlope);

    virtual void Start() override;
public:
    virtual void _CALLBACK OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination);
    virtual void _CALLBACK OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination);

private:
    BurnObjectScript* mBurnScript;
    PlayerController* mController;

};

