#pragma once
#include "Component/Component.h"

class DynamicBehaviorCallback;
class ControllerEventCallback;

class PlayerController : public Component
{
    enum SlopeMode
    {
        Ride, Slide
    };
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
    float t;
    ///move///
    void KeyboardMove();
    void PadMove();
    void SetMoveSpeed(float _speed) { mMoveSpeed = _speed; }
    void CheckOnGround();

    bool GetIsOnGround();
    void SetSlopeMode(SlopeMode _mode);

public:
    ///Jump///
    void SetJumpSpeed(float _speed) { mJumpSpeed = _speed; }
    void JumpUpdate();
    void StartJump();

    //Force
    void GravityUpdate();

    //Collision
    void CheckNowColliding();
public:
    virtual json Serialize();
    virtual void Deserialize(json& j);

public:
    void SetMaterial(std::string _name);
private:
	PxCapsuleController* mCapsuleController;
	PxControllerFilters mCharacterControllerFilters;
    PxCapsuleControllerDesc mCapsuleDesc;
    DynamicBehaviorCallback* mBehaviorCallback;
    ControllerEventCallback* mEventCallback;

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
   
    //Movement
	float mMoveSpeed = 10.f;
    PxVec3 mDisplacement = PxVec3(0.f, 0.f, 0.f);
    bool mIsOnGround = false;
    SlopeMode mSlopeMode = SlopeMode::Ride;
    int mSlopeModeIdx = 0;

    //Jump
    float mJumpInitElapsedTime = 0.f;
    float mJumpSpeed = 0.13f;
    float mJumpDuration = 0.9f; //editable
    float mJumpInputDuration = 0.1f;
    float mJumpElapsedTime = 0.f;
    float mJumpInputElapsedTime = 0.f;

    enum eJumpState {None, InitJump, Jumping};
    eJumpState mJumpState = None;
    
    //Force
    float mGravity = 80.f;

    //Actor and Shapes
    Rigidbody* mRigid = nullptr;
    std::vector<Collider*> mColliders;

    //Colliding
    std::unordered_map<PxActor*, bool> mActorsColliding;

    //Material
    std::vector<std::string> mMaterials;
    int mMaterialIdx = 0;
public:
    virtual void EditorRendering(EditorViewerType _type) override;

    friend class ControllerEventCallback;

};

