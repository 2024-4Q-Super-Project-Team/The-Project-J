#include "pch.h"
#include "Light.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "World/WorldManager.h"
#include "Graphics/GraphicsManager.h"

#include "World/World.h"
#include "World/Light/LightSystem.h"

Light::Light(Object* _owner)
    : Component(_owner)
{
    mType = eComponentType::LIGHT;
    SetLightType(eLightType::Direction);
    mLightProp.Direction = Vector4(0, 0, 1, 0);

    // Depth Stencil Texture 생성
    D3D11_TEXTURE2D_DESC texesc = {};
    texesc.Format = DXGI_FORMAT_R32_TYPELESS;
    texesc.Width = 8192;
    texesc.Height = 8192;
    texesc.MipLevels = 1;
    texesc.ArraySize = 1;
    texesc.SampleDesc.Count = 1;
    texesc.Usage = D3D11_USAGE_DEFAULT;
    texesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texesc.SampleDesc.Count = 1;
    texesc.SampleDesc.Quality = 0;
    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&texesc);

    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    mShadowDSV = new D3DGraphicsDSV(pTexture, &DSVDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;
    mShadowSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);

    SAFE_RELEASE(pTexture);

    // Viewport 생성
    mShadowViewport = new D3DGraphicsViewport(0.0f, 0.0f, 8192.0f, 8192.0f);
}

Light::~Light()
{
}

void Light::Start()
{
}

void Light::Tick()
{
}

void Light::FixedUpdate()
{
}

void Light::PreUpdate()
{
}

void Light::Update()
{
}

void Light::PostUpdate()
{
    GameManager::GetCurrentWorld()->GetLightSystem()->AddLight(mLightProp);
}

void Light::PreRender()
{
    // 그림자 계산 수행
    if (mShadowViewport && mShadowDSV)
    {
        mShadowViewport->Bind();
        mShadowDSV->Bind();
    }
}

void Light::Render()
{

}

void Light::Draw(Camera* _camera)
{
}

void Light::PostRender()
{
}

void Light::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
}

void Light::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Light" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::string name = Helper::ToString(gameObject->GetName());
        ImGui::DragFloat3((" Direction##" + name).c_str(), &mLightProp.Direction.x, 0.05f, -1.0f, 1.0f);
        ImGui::NewLine();
        ImGui::DragFloat3((" Radiance##" + name).c_str(), &mLightProp.Radiance.r, 0.05f, -1.0f, 1.0f);
        /*ImGui::NewLine();
        ImGui::ColorEdit3(("Diffuse##" + name).c_str(), &mLightProp.DiffuseRGB.r);
        ImGui::NewLine();
        ImGui::ColorEdit3(("Ambient##" + name).c_str(), &mLightProp.AmbientRGB.r);
        ImGui::DragFloat(("AmbientStrength##" + name).c_str(), &mLightProp.AmbientStrength, 0.05f, 0.0f, 1.0f);
        ImGui::NewLine();
        ImGui::ColorEdit3(("Specular##" + name).c_str(), &mLightProp.SpecularRGB.r);*/
    }
}
