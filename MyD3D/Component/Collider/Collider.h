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
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void DrawObject(Matrix& _view, Matrix& _projection) {}
    virtual void DrawShadow(Light* _pLight) {}
    virtual void DrawWire() override;
    virtual eBlendModeType GetBlendMode() override { return eBlendModeType::OPAQUE_BLEND; }
    virtual eRasterizerStateType GetCullingMode() override { return eRasterizerStateType::BACKFACE_CULLING; }
    virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
    void SetPxShape(PxShape* _shape) { mShape = _shape; AddShapeToRigidbody(); }
    virtual Vector3 GetSize() { return Vector3(1.0f, 1.0f, 1.0f); }
public:
    virtual json Serialize() { return json(); }
    virtual void Deserialize(json& j) {}

    virtual MeshResource* GetMesh() { return nullptr; }
    virtual MaterialResource* GetMaterial() { return nullptr; }

    static bool bDrawMode;
public:
    Object* GetOwner() { return gameObject; }
    void AddTriggerOther(Collider* _collider);
    void RemoveTriggerOther(Collider* _collider);

protected:
    void SetIsTrigger();
    virtual void SetPosition() {}
    virtual void SetRotation() {}
    void AddShapeToRigidbody();
    void SetMaterial(std::string _name);

private:
    void CheckTriggers();
protected:
	PxShape* mShape = nullptr;
    bool mIsTrigger;
    Vector3 mPosition;
    Vector3 mRotation;
    Quaternion mQuatRotation;
    Rigidbody* mRefRigidbody;

    int mMaterialIdx = 0;
    std::vector<std::string> mMaterials;
protected:
    const Color mBaseColor = Color(0, 1, 0, 1);
    const Color mIntersectColor = Color(1, 0, 0, 1);

	friend class Rigidbody;
	friend class PlayerController;

    bool addedToRigid = false;

private:
    std::unordered_set<Collider*> mTriggerOthers;
};

