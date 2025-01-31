#pragma once

class ButtonScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(ButtonScript);
public:
    void Start() override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnButtonPressed();
private:
    bool isPressed;
};

