#pragma once
#include "Component/Component.h"
#include "Component/Renderer/Renderer.h"

class AnimationResource;
class AnimationNode;
class Transform;

// ���� ��������Ʈ���� Ʈ�������� ����
class Animator
    : public Component
{
public:
    explicit Animator(Object* _owner);
    virtual ~Animator();
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
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    void SetAnimation(ResourceHandle _handle);
    void SetAnimation(AnimationResource* _pAnim);
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j);
private:
    void        CalculateAnimationTramsform(Transform* _pBone);
    Vector3     CalculateAnimationPosition(AnimationNode* _pChannel);
    Quaternion  CalculateAnimationRotation(AnimationNode* _pChannel);
    Vector3     CalculateAnimationScaling(AnimationNode* _pChannel);
    // TODO : AudioSourceó�� ���̺�� �ΰ� �����ϴ°� ���� ������? ��� ����
private:
    ResourceHandle      mAnimationHandle;
    AnimationResource*  mActiveAnimation;
    BOOL  isPlaying;
    FLOAT mDuration;
    FLOAT mFrameRateScale;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};