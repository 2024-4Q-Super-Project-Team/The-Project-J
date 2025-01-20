#pragma once
#include "Component/Component.h"

enum eTransformType
{
    POSITION,
    ROTATION,
    SCALE,
};

class Object;

class Transform
    : public Component
{
public:
    explicit Transform(Object* _owner);
    virtual ~Transform();
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
    void UpdatePxTransform();
    void UpdateFromPxTransform(PxTransform pxTransform);
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) override;
public:
    // Radian���� �޴� Vector3
    inline void SetEulerAnglesFromRadian(const Vector3& euler) {
        rotation = Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
    }
    // Degree�� �޴� Vector3
    inline void SetEulerAnglesFromDegree(const Vector3& euler) {
        rotation = Quaternion::CreateFromYawPitchRoll(Degree::ToRadian(euler.y), Degree::ToRadian(euler.x), Degree::ToRadian(euler.z));
    }
    // Euler�� ��������
    inline Vector3 GetEulerAngles() const { 
        return rotation.ToEuler();
    }
public:
    inline Transform* GetParent() const { return mParent; }
    inline Transform* GetChild(UINT _index = 0) {
        if (_index >= mChildren.size()) return nullptr; // ������ ����� nullptr ��ȯ
        return mChildren[_index];
    }
    inline const std::vector<Transform*>& GetChildren() { return mChildren; }
    inline const Vector3& GetWorldPosition() { return Vector3(mWorldMatrix._41, mWorldMatrix._42, mWorldMatrix._43); }
    inline const Matrix&  GetLocalMatrix() { return mLocalMatrix; }
    inline const Matrix&  GetWorldMatrix() { return mWorldMatrix; }
    inline const Vector3  Forward() { return GetWorldMatrix().Forward(); }
    inline const Vector3  Up() { return GetWorldMatrix().Up(); }
    inline const Vector3  Right() { return GetWorldMatrix().Right(); }
    inline const Vector3  Backward() { return GetWorldMatrix().Backward(); }
    inline const PxTransform  GetPxTransform() { return mPxTransform; }
public:
    // ��Ƽ�÷��װ� True�� ������Ʈ�� �Ѵ�.
    void UpdateMatrix();
    // �θ� �����Ѵ�.
    void SetParent(Transform* _parent);

    virtual json Serialize();
    virtual void Deserialize(json& j);
public:
    // Position to Vector3
    Vector3			position;
    // Rotation To Quaternion
    Quaternion		rotation;
    // Scale to Vector3
    Vector3			scale;
private:
    // ====================================
    //
    // ��ġ ����============================
    Matrix			mLocalMatrix; // �����ǥ
    Matrix			mWorldMatrix; // ������ǥ
    // ====================================
    //
    // ���� ����============================
    Transform* mRootParent;
    Transform* mParent;
    std::vector<Transform*> mChildren;
    //UINT mHierarchyLevel; // ���� ����
    // ====================================
    PxTransform mPxTransform{};

public:
    void EditorRendering(EditorViewerType _viewerType);
};