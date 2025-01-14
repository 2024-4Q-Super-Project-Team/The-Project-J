#pragma once
#include "Component/Component.h"

class AnimationResource;
class AnimationNode;
class Transform;

// 하위 본오브젝트들의 트랜스폼을 변형
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
public:
    void SetAnimation(std::shared_ptr<AnimationResource> _pAnim);

    virtual json Serialize() override;
    virtual void Deserialize(json& j);
private:
    void        CalculateAnimationTramsform(Transform* _pBone);
    Vector3     CalculateAnimationPosition(AnimationNode* _pChannel);
    Quaternion  CalculateAnimationRotation(AnimationNode* _pChannel);
    Vector3     CalculateAnimationScaling(AnimationNode* _pChannel);

private:
    std::shared_ptr<AnimationResource> mActiveAnimation;

    bool  isPlay;
    float mDuration;
};