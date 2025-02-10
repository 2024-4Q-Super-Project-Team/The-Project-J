#pragma once
#include "ButtonScript.h"

class RaiseGroundButtonScript : public ButtonScript
{
public:
    void Start() override;
public:
    explicit RaiseGroundButtonScript(Object* _owner);
    virtual ~RaiseGroundButtonScript() = default;
public:
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnTriggerExit(Collider* _origin, Collider* _destination) override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed() override;
    virtual void OnButtonReleased() override;
private:
    //Object* wall1;
    //Object* wall2;

    //std::wstring GetWallTag(int index);

    std::vector<Object*> walls;
    std::vector<Vector3> originalPositions;
    bool isPressed;
    std::vector<std::wstring> GetWallTags();
protected:
    virtual bool CanInteract(Object* _object) override; // 상호작용 할 수 있는지 판단 (1P, 2P, 물체 등)
};