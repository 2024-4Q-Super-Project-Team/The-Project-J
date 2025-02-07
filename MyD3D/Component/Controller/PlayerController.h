#pragma once
#include "Component/Component.h"

class DynamicBehaviorCallback;
class ControllerEventCallback;


class PlayerController : public Component
{
public:
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
    inline bool IsGround() { return mIsOnGround; }
    ///move///
    void Move(Vector3 _displacement);
    void SetMoveForceX(FLOAT _x);
    void SetMoveForceY(FLOAT _y);
    void SetMoveForceZ(FLOAT _z);
    void AddMoveForceY(FLOAT _y);
    
    inline void SetGravity(FLOAT _val) { mGravity = _val; }
    inline void SetMaxGravity(FLOAT _val) { mMaxGravity = _val; }
    void SetSlopeMode(SlopeMode _mode);
    float GetGravity() { return mGravity; }
    SlopeMode GetSlopeMode() { return mSlopeMode; }
    Rigidbody* GetRigidbody() { return mRigid; }
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
    float mGravity = 0.5f;
    float mMaxGravity = 100.0f;
    float mGravityElapsedTime = 0.f;

    //Actor and Shapes
    Rigidbody* mRigid = nullptr;
    std::vector<Collider*> mColliders;

    //Colliding
    std::unordered_map<PxActor*, bool> mActorsColliding;
    std::unordered_map<PxController*, bool> mControllersColliding;

    //Material
    std::vector<std::string> mMaterials;
    int mMaterialIdx = 0;

    //ray for onground
    class CustomRaycastFilter* mRayFilter;
public:
    virtual void EditorRendering(EditorViewerType _type) override;

    friend class ControllerEventCallback;

};

class CustomRaycastFilter : public PxQueryFilterCallback
{
public:
    // PxQueryFilterCallback의 필터 함수 구현
    PxQueryHitType::Enum preFilter(
        const PxFilterData& filterData,
        const PxShape* shape,
        const PxRigidActor* actor,
        PxHitFlags& queryFlags) override
    {
        // Trigger Shape는 제외
        if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
        {
            return PxQueryHitType::eNONE; // 충돌 무시
        }

        // Trigger가 아니면 충돌 처리
        return PxQueryHitType::eBLOCK;
    }

    virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
    {
        return PxQueryHitType::eBLOCK;
    }

};
