#pragma once
#include "Component/Component.h"

//반드시 Rigidbody가 있는 상태에서 추가해야 합니다.
class Collider : public Component
{
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

	void SetLocalPosition(Vector3 pos);
	void SetRotation(Vector3 rotation);

	void SetIsTrigger(bool isTrigger);
	bool GetIsTrigger() { return mIsTrigger; }

public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
	bool mIsTrigger = false;

protected:
	PxShape* mShape = nullptr;

	friend class Rigidbody;
};

