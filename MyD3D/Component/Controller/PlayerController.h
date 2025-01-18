#pragma once
#include "Component/Component.h"

class PlayerController : public Component
{
public:
    explicit PlayerController(Object* _owner);
    virtual ~PlayerController();
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera) override;
    virtual void PostRender() override;
public:
    virtual json Serialize();
    virtual void Deserialize(json& j);

private:
	static PxControllerManager* ControllerManager;
	PxCapsuleController* mCapsuleController;
	PxControllerFilters mCharacterControllerFilters;

	float mSpeed = 0.5f;
	float mJumpSpeed = 30.f;
	float mGravity = 9.8f;
	PxVec3 mMoveDirection = PxVec3(0.f, 0.f, 0.f);
};

