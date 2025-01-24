#pragma once
#include "Component/Component.h"

//반드시 Rigidbody가 있는 상태에서 추가해야 합니다.
class Collider : public Component
               , public IRenderContext
{
public:
    explicit Collider(Object* _owner);
    virtual ~Collider();
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
    virtual void DrawObject(Matrix& _view, Matrix& _projection) = 0;
    virtual void DrawShadow(Light* _pLight)  = 0;
    virtual eBlendModeType GetBlendMode() override { return eBlendModeType::OPAQUE_BLEND; }
    virtual eRasterizerStateType GetCullingMode() override { return eRasterizerStateType::BACKFACE_CULLING; }
    virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
public:
    virtual json Serialize()  = 0;
    virtual void Deserialize(json& j)  = 0;

    static bool bDrawMode;

protected:
    void SetIsTrigger();
    virtual void SetLocalPosition();
    virtual void SetRotation();
    void AddShapeToRigidbody();
protected:
	PxShape* mShape = nullptr;
    bool mIsTrigger;
    Vector3 mPosition;
    Vector3 mRotation;
    Quaternion mQuatRotation;
    Rigidbody* mRefRigidbody;
protected:
    const Color mBaseColor = Color(0, 1, 0, 1);
    const Color mIntersectColor = Color(1, 0, 0, 1);

	friend class Rigidbody;
};

