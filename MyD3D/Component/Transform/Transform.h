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
    // Radian으로 받는 Vector3
    inline void SetEulerAnglesFromRadian(const Vector3& euler) {
        rotation = Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
    }
    // Degree로 받는 Vector3
    inline void SetEulerAnglesFromDegree(const Vector3& euler) {
        rotation = Quaternion::CreateFromYawPitchRoll(Degree::ToRadian(euler.y), Degree::ToRadian(euler.x), Degree::ToRadian(euler.z));
    }
    // Euler로 가져오기
    inline Vector3 GetEulerAngles() const { 
        return rotation.ToEuler();
    }
public:
    inline Transform* GetParent() const { return mParent; }
    inline Transform* GetChild(UINT _index = 0) {
        if (_index >= mChildren.size()) return nullptr; // 범위를 벗어나면 nullptr 반환
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
    // 더티플래그가 True면 업데이트를 한다.
    void UpdateMatrix();
    // 부모를 변경한다.
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
    // 위치 정보============================
    Matrix			mLocalMatrix; // 상대좌표
    Matrix			mWorldMatrix; // 월드좌표
    // ====================================
    //
    // 계층 정보============================
    Transform* mRootParent;
    Transform* mParent;
    std::vector<Transform*> mChildren;
    //UINT mHierarchyLevel; // 계층 깊이
    // ====================================
    PxTransform mPxTransform{};

public:
    void EditorRendering(EditorViewerType _viewerType);
};