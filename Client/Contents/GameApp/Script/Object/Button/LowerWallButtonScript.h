#pragma once
#include "ButtonScript.h"

class LowerWallButtonScript : public ButtonScript
{
public:
    void Start() override;
public:
    explicit LowerWallButtonScript(Object* _owner);
    virtual ~LowerWallButtonScript() = default;
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
    virtual bool CanInteract(Object* _object) override; // 상호작용 할 수 있는지 판단 (1P, 2P, 물체 등)
};