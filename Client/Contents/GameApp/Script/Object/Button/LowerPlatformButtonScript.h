#pragma once
#include "ButtonScript.h"

class LowerPlatformCollisionScript;

class LowerPlatformButtonScript : public ButtonScript
{
public:
	void Start() override;
    void Update() override;
public:
    explicit LowerPlatformButtonScript(Object* _owner);
    virtual ~LowerPlatformButtonScript() = default;
public:
    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    void _CALLBACK OnCollisionEnterCallback(Rigidbody* _origin, Rigidbody* _destination);

    void RotatePlatform();

private:
    Object* platform;
    Object* mButtonObject;
    Object* mLowerPlatformObject;

    LowerPlatformCollisionScript* mButtonCollisionScript;

    float mAnimTime = 3.f;
    float mAnimElapsedTime = 0.f;

    bool mRotated = false;
    bool isCollided = false;
};