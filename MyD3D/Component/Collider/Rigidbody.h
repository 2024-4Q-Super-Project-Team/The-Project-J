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

public:
    //PxRigidActor 설정하는 함수들
    void AddShape(PxShape* _shape) { mRigidActor->attachShape(*_shape); }

public:
    void SetIsDynamic(bool _isDynamic) { mIsDynamic = _isDynamic; }
    void SetMass(float mass);

public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
	PxRigidActor* mRigidActor;
	bool mIsDynamic = false;

	friend class Collider;
};

