#pragma once
class ButtonScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(ButtonScript);
public:
    void Start() override;
    virtual void OnCollisionEnter(Collider* _origin, Collider* _destination) override;
private:

};

