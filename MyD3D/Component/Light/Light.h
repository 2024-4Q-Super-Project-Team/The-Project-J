#pragma once
#include "Component/Component.h"

enum class eLightType
{
    Direction,
    Point,
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
public:
    virtual void Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable) override;
public:
    inline void SetLightDirection(Vector4 _xyz) { mLightProp.Direction = _xyz; }
    inline void SetLightType(eLightType _type) { mLightProp.LightType = static_cast<INT>(_type); }
    inline void SetAmbientColor(ColorF _color) { mLightProp.AmbientRGB = _color; }
    inline void SetDiffuseColor(ColorF _color) { mLightProp.DiffuseRGB = _color; }
    inline void SetSpecularColor(ColorF _color) { mLightProp.SpecularRGB = _color; }
public:
    inline auto GetShadowViewport() { return mShadowViewport; }
    inline auto GetShadowDSV() { return mShadowDSV; }
    inline auto GetShadowSRV() { return mShadowSRV; }
    inline auto GetProperty() { return mLightProp; }
public:
    virtual json Serialize() override;
    virtual void Deserialize( json& j) override;

private:
    LightProperty        mLightProp;
    D3DGraphicsDSV*      mShadowDSV;
    D3DGraphicsSRV*      mShadowSRV;
    D3DGraphicsViewport* mShadowViewport;

    // Light의 Near, Far
    float mLightNear = 50.0f;
    float mLightFar = 15000.0f;
    // 카메라 -> 그림자(광원 지점) 거리
    float mCameradDist = 0.0f;
    // 그림자 -> 조명 거리
    float mUpDist = 3000.0f;
public:
    virtual void EditorRendering() override;
};

