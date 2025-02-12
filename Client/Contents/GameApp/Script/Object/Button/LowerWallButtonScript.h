#pragma once
#include "ButtonScript.h"

class LowerWallButtonScript : public ButtonScript
{
public:
    void Start() override;
    void Update() override;
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
    Object* box;
    Vector3 buttonPosition;

    std::wstring GetWallTag();

    float activationDistance = 50.0f;
    bool isWithinDistance = false;
protected:
    virtual bool CanInteract(Object* _object) override; // 상호작용 할 수 있는지 판단 (1P, 2P, 물체 등)
};