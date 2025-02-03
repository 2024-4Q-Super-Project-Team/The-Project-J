#pragma once
#include "ButtonScript.h"

class LowerPlatformButtonScript : public ButtonScript
{
public:
    explicit LowerPlatformButtonScript(Object* _owner);
    virtual ~LowerPlatformButtonScript() = default;
public:
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed() override;
	virtual void OnButtonReleased() override;
private:
protected:
    virtual bool CanInteract(Object* _object) override; // ��ȣ�ۿ� �� �� �ִ��� �Ǵ� (1P, 2P, ��ü ��)
private:
    Object* platform;
};