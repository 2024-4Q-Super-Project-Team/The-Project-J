#pragma once
#include "Component/Component.h"
#include "Helper/Dotween/DotTween.h"
#include "Helper/Dotween/EasingFunction.h"

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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    void UpdatePxTransform();
    void UpdateFromPxTransform(PxTransform pxTransform);
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) override;

public:
    void SetEulerAngles(const Vector3& euler) { rotation = Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z); }
    Vector3 GetEulerAngles() const { return rotation.ToEuler(); }

// Dotween 관련
// 회전
private:
    bool isRotating = false;
	bool isLookingAt = false;
    float rotationDuration = 2.0f;
    float rotationElapsedTime = 0.0f;
    Dotween::EasingEffect easingEffect;
    Quaternion startRotation;
    Quaternion endRotation;
// 움직임
private:
	bool isMoving = false;
	float moveDuration = 2.0f;
	float moveElapsedTime = 0.0f;
	Vector3 startPosition;
	Vector3 endPosition;

public:
    inline Transform* GetParent() const { return mParent; }
    inline Transform* GetRootParent() { return mParent != nullptr? GetRootParent() : this; }
    inline bool       IsBelong(Transform* _dest)
    {
        if (this == nullptr || mParent == nullptr)
            return false; // 부모가 없거나 자신이 nullptr이면 속해 있지 않음

        if (mParent == _dest)
            return true; // 부모가 _dest와 같으면 속해 있음

        return mParent->IsBelong(_dest); // 재귀적으로 부모 계층 확인
    }
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
	inline const float GetHeight() { return scale.y; }
    Vector3 LocalToWorld(const Vector3& localPosition) const;
public:
    // 더티플래그가 True면 업데이트를 한다.
    void UpdateMatrix();
    // 부모를 변경한다.
    void SetParent(Transform* _parent);
    void SetLocalMatrix(Matrix& _matrix);

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
    // 위치 정보============================
    Matrix			mLocalMatrix; // 상대좌표
    Matrix			mWorldMatrix; // 월드좌표
    // 계층 정보============================
    Transform* mRootParent;
    Transform* mParent;
    std::vector<Transform*> mChildren;
    // ====================================
    PxTransform mPxTransform{};

public:
    void EditorRendering(EditorViewerType _viewerType);

// dotween 함수
public:
    // dotween 함수
    void Rotate90(float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutBounce);
    void RotateByPivot(const Vector3& pivot, const Vector3& axis, float angle, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutBounce);
	void LookAt(const Vector3& targetPosition, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);
	void MoveTo(const Vector3& targetPosition, float duration, Dotween::EasingEffect easingEffect = Dotween::EasingEffect::OutSine);

private:
    void UpdateLookAt(float t, Dotween::EasingEffect easingEffect);
    void UpdateRotation(float t, Dotween::EasingEffect easingEffect);
	void UpdateMove(float t, Dotween::EasingEffect easingEffect);
    


};