#pragma once

class JumpPadScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(JumpPadScript);
public:
    void Start() override;
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;

public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    SerializeField(FLOAT, mJumpForce, 5.0f);
    float mJumpTimeRatio = 0.5f;
};