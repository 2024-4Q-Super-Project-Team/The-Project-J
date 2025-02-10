#pragma once
#include "Component/Renderer/Renderer.h"

class Transform;

class BoneAttacher : public Component
{
public:
    explicit BoneAttacher(Object* _owner);
    virtual ~BoneAttacher();
public:
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
    inline void SetDestBone(Transform* _dstBone) { mDestBone = _dstBone; }
    inline void SetOffsetPosition(Vector3 _vec3) { mOffsetPosition = _vec3; }
    inline void SetOffsetRotation(Quaternion _quat) { mOffsetRotation = _quat; }
    inline void SetOffsetScaling(Vector3 _vec3) { mOffsetScale = _vec3; }
private:
    // Owner���� DestBone�� �ٴ´�.
    Transform* mDestBone;

    Vector3     mOffsetPosition = Vector3::Zero;
    Quaternion  mOffsetRotation = Quaternion::Identity;
    Vector3     mOffsetScale = Vector3::One;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
};

