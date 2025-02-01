#pragma once
#include "Component/Component.h"
#include "Component/Renderer/Renderer.h"

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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    void SetAnimation(ResourceHandle _handle);
    void SetAnimation(AnimationResource* _pAnim);
public:
    inline void Play() {    // 처음으로 돌아가서 재생
        isPlaying = TRUE;
        mDuration = 0.0f;
    }
    inline void Stop() {    // 처음으로 돌아가서 정지
        isPlaying = FALSE;
        mDuration = 0.0f;
    }
    inline void Pause() {   // 중지
        isPlaying = FALSE;
    }
    inline void Resume() {  // 이어서재생
        isPlaying = TRUE;
    }
    inline void SetFrame(FLOAT _val) { mDuration = _val; }
    inline void SetFremeRateScale(FLOAT _val) { mFrameRateScale = _val; }
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j);
private:
    void        CalculateAnimationTramsform(Transform* _pBone);
    Vector3     CalculateAnimationPosition(AnimationNode* _pChannel);
    Quaternion  CalculateAnimationRotation(AnimationNode* _pChannel);
    Vector3     CalculateAnimationScaling(AnimationNode* _pChannel);
    // TODO : AudioSource처럼 테이블로 두고 관리하는게 낫지 않을까? 라는 생각
private:
    ResourceHandle      mAnimationHandle;
    AnimationResource*  mActiveAnimation;
    BOOL  isPlaying;
    BOOL  isLoop;
    FLOAT mDuration;
    FLOAT mFrameRateScale;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};