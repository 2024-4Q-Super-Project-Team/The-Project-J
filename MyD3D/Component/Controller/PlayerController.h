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
    void Move(Vector3 _displacement);
    void CheckOnGround();

    bool GetIsOnGround();
    void SetSlopeMode(SlopeMode _mode);

public:
    ///Jump///

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

    //Capsule Controller
    float mHeight = 4;
    float mRadius = 2;
    float mContactOffset = 0.1f;
    float mSlopeLimit = 0.707f;
    float mStepOffset = 0.5f;
   
    //Movement
    PxVec3 mDisplacement = PxVec3(0.f, 0.f, 0.f);
    bool mIsOnGround = false;

    //SlopeMode
    SlopeMode mSlopeMode = SlopeMode::Ride;
    int mSlopeModeIdx = 0;

    //Force
    float mGravity = 80.f;
    float mGravityElapsedTime = 0.f;

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

