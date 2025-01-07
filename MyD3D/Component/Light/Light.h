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
private:
    LightProperty   mLightProp;
    D3DGraphicsDSV* mShadowDSV;
    D3DGraphicsSRV* mShadowSRV;
    D3DGraphicsViewport* mShadowViewport;
public:
    virtual void EditorRendering() override;
};

