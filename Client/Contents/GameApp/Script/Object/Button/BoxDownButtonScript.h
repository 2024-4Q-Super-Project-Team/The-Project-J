#pragma once
#include "ButtonScript.h"

class BoxDownButtonScript : public ButtonScript
{
public:
    explicit BoxDownButtonScript(Object* _owner);
    virtual ~BoxDownButtonScript() = default;

    virtual void Start() override;
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnTriggerExit(Collider* _origin, Collider* _destination) override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed() override;
    virtual void OnButtonReleased() override;

private:
    Object* stageWall;
    Object* door1;
    Object* door2;
    Animator* mLeftDoorAnimator = nullptr;
    Animator* mRightDoorAnimator = nullptr;

    std::wstring GetDoorTag(int doorNumber);
    void RotateDoor(Object* door, float angle);
};