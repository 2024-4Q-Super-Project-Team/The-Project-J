#pragma once
#include "Component/Component.h"

class PlayerBehaviorCallback;

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
    float t;
    ///move///
    void KeyboardMove();
    void PadMove();
    void SetMoveSpeed(float _speed) { mMoveSpeed = _speed; }


public:
    ///Jump///
    void SetJumpSpeed(float _speed) { mJumpSpeed = _speed; }
    void JumpUpdate();
    void StartJump();

    //Force
    void GravityUpate();
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
   
    //Movement
	float mMoveSpeed = 10.f;
    PxVec3 mDisplacement = PxVec3(0.f, 0.f, 0.f);

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

    //Material
    std::vector<std::string> mMaterials;
    int mMaterialIdx = 0;
public:
    virtual void EditorRendering(EditorViewerType _type) override;

};

