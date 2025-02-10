#pragma once
#include "ButtonScript.h"

class RaisePlatformButtonScript : public ButtonScript
{
public:
    void Start() override;
public:
    explicit RaisePlatformButtonScript(Object* _owner);
    virtual ~RaisePlatformButtonScript() = default;
public:
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnTriggerExit(Collider* _origin, Collider* _destination) override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed() override;
    virtual void OnButtonReleased() override;
private:
    Object* wall;
    std::wstring GetWallTag();
protected:
    virtual bool CanInteract(Object* _object) override; // ��ȣ�ۿ� �� �� �ִ��� �Ǵ� (1P, 2P, ��ü ��)
};