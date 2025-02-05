#pragma once
#include "Component/Component.h"

enum class eLightType
{
    Direction,
    Point,
    Spot,
};

class Light
    : public Component
{
public:
    explicit Light(Object* _owner);
    virtual ~Light();
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
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
private:
    void AllocShadowMap();
    void FreeShadowMap();
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) override;
public:
    inline void SetLightDirection(Vector4 _xyz) { mLightProp.Direction = _xyz; }
    inline void SetLightType(eLightType _type) { mLightProp.LightType = static_cast<INT>(_type); }
    inline void SetLightRange(float _range) { mLightProp.LightRadius = _range; }
    inline void SetLightCutOff(float _cutOff) { mLightProp.LightCutOff = _cutOff; }
    inline void SetLightStrengh(float _strengh) { mLightProp.LightStrengh = _strengh; }
    inline void SetLightColor(ColorF _color) { mLightProp.Radiance = _color; }
    inline void SetLightNear(float _val) { mLightNear = _val; }
    inline void SetLightFar(float _val) { mLightFar = _val; }
    inline void SetShadowDistance(float _val) { mShadowDistance = _val; }
public:
    inline auto GetShadowViewport() { return mShadowViewport; }
    inline auto GetShadowRenderTarget() { return mShadowRenderTarget; }
    inline auto GetProperty() { return mLightProp; }
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
private:
    LightProperty           mLightProp;
    D3DBitmapRenderTarget*  mShadowRenderTarget;
    D3DGraphicsViewport*    mShadowViewport;
    // Light�� Near, Far
    float mLightNear = 50.0f;
    float mLightFar = 15000.0f;
    // ī�޶� -> �׸���(���� ����) �Ÿ�
    float mCameraDist = 0.0f;
    // �׸��� -> ���� �Ÿ�
    float mUpDist = 3000.0f;
	// �׸��� �� �ػ�
	float mShadowResolution;
    // �׸��� ���� ����
    float mShadowDistance;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};

