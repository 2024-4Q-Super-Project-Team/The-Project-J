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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    virtual json Serialize();
    virtual void Deserialize(json& j);

public:
    void SetMaterial(std::string _name);
private:
	static PxControllerManager* ControllerManager;
	PxCapsuleController* mCapsuleController;
	PxControllerFilters mCharacterControllerFilters;

    //Capsule Controller
    float mHeight = 4;
    float mRadius = 2;
    float mContactOffset = 0.1f;
    float mSlopeLimit = 0.707f;
    float mStepOffset = 0.5f;
    int mMaterialIdx = 0;
    //Movement
	float mMoveSpeed = 0.5f;
	float mJumpSpeed = 30.f;
	float mGravity = 9.8f;
	PxVec3 mMoveDirection = PxVec3(0.f, 0.f, 0.f);


    std::vector<std::string> mMaterials;

public:
    virtual void EditorRendering(EditorViewerType _type) override;

};

