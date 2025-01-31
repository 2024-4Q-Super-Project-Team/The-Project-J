#pragma once

class JumpPadScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(JumpPadScript);
public:
    void Start() override;
    virtual void _CALLBACK OnCollisionEnter(Collider* _origin, Collider* _destination) override;
private:
	float mJumpForce = 20.0f;
    //SerializeField(FLOAT, mJumpForce, 20.0f);
};