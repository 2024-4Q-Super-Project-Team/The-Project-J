#pragma once
#include "Component/Component.h"

class Rigidbody : public Component
{
public:
    explicit Rigidbody(Object* _owner);
    virtual ~Rigidbody();
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
    //PxRigidActor 설정하는 함수들
    void AddShape(PxShape* _shape) { mRigidActor->attachShape(*_shape); }
    void SetPxActor(PxRigidActor* _actor) { mRigidActor = _actor; }
public:
    void SetMaterial(std::string _name); 

    void SetIsDynamic(bool _isDynamic);
    void SetMass(float mass);
    void SetIsKinematic(bool b);
    void SetDisableGravity(bool b);
    void SetFreezePosition(bool bx, bool by, bool bz);
    void SetFreezeRotation(bool bx, bool by, bool bz);
    void AddForce(Vector3 force, PxForceMode::Enum forceMode = PxForceMode::eFORCE);
public:
    Object* GetOwner() { return gameObject; }

    void AddContactOther(Rigidbody* _rigid);
    void RemoveContactOther(Rigidbody* _rigid);
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;

private:
    void CheckContacts();
private:
	PxRigidActor* mRigidActor;
	bool mIsDynamic = false;
    bool mIsKinematic = false;
    bool mDisableGravity = false;
    float mMass = 1.f;

    bool mFreezePosition[3];
    bool mFreezeRotation[3];
public:
    virtual void EditorRendering(EditorViewerType _type) override;

    friend class Collider;
    friend class PlayerController;

private:
    std::unordered_set<Rigidbody*> mContactOthers;
};

