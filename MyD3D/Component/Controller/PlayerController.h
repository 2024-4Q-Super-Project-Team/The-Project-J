#pragma once
#include "Component/Component.h"

class PlayerBehaviorCallback;

enum class MovementFlag : uint8_t
{
    SlideOnPlane = 1 << 0,   // 평면에서 슬라이드
    SlideOnSlope = 1 << 1    // 경사면에서 슬라이드
};

using MovementFlags  = uint8_t;

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

    virtual void _CALLBACK OnEnable();
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;

public:
    void SetForwardKey(Key _key);
    void SetBackwardKey(Key _key);
    void SetLeftKey(Key _key);
    void SetRightKey(Key _key);
    void SetJumpKey(Key _key);
    void SetJumpSpeed(float speed) { mJumpSpeed = speed; }
    void StartJump();
public:
    virtual json Serialize();
    virtual void Deserialize(json& j);

public:
    void SetMaterial(std::string _name);
private:
	PxCapsuleController* mCapsuleController;
	PxControllerFilters mCharacterControllerFilters;
    PxCapsuleControllerDesc mCapsuleDesc;
    PlayerBehaviorCallback* mIceBehavior;
    //Key
    int mForwardKeyIdx = 0;
    int mBackwardKeyIdx = 0;
    int mLeftKeyIdx = 0;
    int mRightKeyIdx = 0;
    int mJumpKeyIdx = 0;
    std::vector<std::string> mStrKeys;

    //Capsule Controller
    float mHeight = 4;
    float mRadius = 2;
    float mContactOffset = 0.1f;
    float mSlopeLimit = 0.707f;
    float mStepOffset = 0.5f;
    int mMaterialIdx = 0;
    //Movement
	float mMoveSpeed = 10.f;
	float mJumpSpeed = 0.13f;
	float mGravity = 98.f;
    std::vector<std::string> mMaterials;


    PxVec3 mPrevPosition;
    PxVec3 mDisplacement = PxVec3(0.f, 0.f, 0.f);
    PxVec3 mVelocity = PxVec3(0.f, 0.f, 0.f);
    float mAcceleration = 20.f;

    MovementFlags mMovementFlags;

    //Jump
    float mJumpInitElapsedTime = 0.f;

    float mJumpDuration = 0.9f; //editable
    float mJumpInputDuration = 0.1f;
    float mJumpElapsedTime = 0.f;
    float mJumpInputElapsedTime = 0.f;

    enum eJumpState {None, InitJump, Jumping};
    eJumpState mJumpState = None;

    //Actor and Shapes
    Rigidbody* mRigid = nullptr;
    std::vector<Collider*> mColliders;

public:
    virtual void EditorRendering(EditorViewerType _type) override;

};

