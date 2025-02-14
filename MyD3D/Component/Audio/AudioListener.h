#pragma once
#include "Component/Component.h"

class AudioResource;

class AudioListener
    : public Component
{
public:
    explicit AudioListener(Object* _owner);
    virtual ~AudioListener();
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
    // 거리 감쇠율
    inline void SetDistanceAttenuationScale(float _val) { mDistanceAttenuationScale = Clamp(_val, 0.0f, 1.0f); }
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j);
public:
    static AudioListener* mMainListener;

    float mDistanceAttenuationScale;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};


//FMOD_VECTOR customCurve[3] = {
//{ 0.0f, 1.0f, 0.0f }, // 가까운 거리 (풀 볼륨)
//{ 10.0f, 0.5f, 0.0f }, // 중간 거리 (볼륨 절반)
//{ 50.0f, 0.0f, 0.0f }  // 먼 거리 (소리 없음)
//};
//channel->set3DCustomRolloff(customCurve, 3);