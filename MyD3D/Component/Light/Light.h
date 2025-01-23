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
    void UpdateLightProperty();
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) override;
public:
    inline void SetLightDirection(Vector4 _xyz) { mLightProp.Direction = _xyz; }
    inline void SetLightType(eLightType _type) { mLightProp.LightType = static_cast<INT>(_type); }
    inline void SetLightRange(float _range) { mLightProp.LightRadius = _range; }
    inline void SetLightCutOff(float _cutOff) { mLightProp.LightCutOff = _cutOff; }
    inline void SetLightColor(ColorF _color) { mLightProp.Radiance = _color; }
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

    // Light의 Near, Far
    float mLightNear = 50.0f;
    float mLightFar = 15000.0f;
    // 카메라 -> 그림자(광원 지점) 거리
    float mCameradDist = 0.0f;
    // 그림자 -> 조명 거리
    float mUpDist = 3000.0f;
	// 그림자 맵 해상도
	float mShadowResolution;
    // 그림자 렌더 범위
    float mShadowDistance;

	static std::queue<D3DBitmapRenderTarget*> mShadowRenderTargetPool;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};

