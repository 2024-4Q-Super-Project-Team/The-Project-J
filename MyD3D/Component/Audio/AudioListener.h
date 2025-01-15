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
public:
    // �Ÿ� ������
    inline void SetDistanceAttenuationScale(float _val) { mDistanceAttenuationScale = Clamp(_val, 0.0f, 1.0f); }
public:
    virtual json Serialize() override { return json(); }
    virtual void Deserialize(json& j) override {}
public:
    static AudioListener* mMainListener;

    float mDistanceAttenuationScale;
};


//FMOD_VECTOR customCurve[3] = {
//{ 0.0f, 1.0f, 0.0f }, // ����� �Ÿ� (Ǯ ����)
//{ 10.0f, 0.5f, 0.0f }, // �߰� �Ÿ� (���� ����)
//{ 50.0f, 0.0f, 0.0f }  // �� �Ÿ� (�Ҹ� ����)
//};
//channel->set3DCustomRolloff(customCurve, 3);