#include "pch.h"
#include "Light.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "World/WorldManager.h"
#include "Graphics/GraphicsManager.h"

#include "World/World.h"
#include "World/Light/LightSystem.h"

float Light::ShadowArea = 4096.0f;

Light::Light(Object* _owner)
    : Component(_owner)
{
    mType = eComponentType::LIGHT;
    SetLightType(eLightType::Direction);
    mLightProp.Direction = Vector4(0, 0, 1, 0);
    // Viewport 생성
    mShadowViewport = new D3DGraphicsViewport(0.0f, 0.0f, ShadowArea, ShadowArea);
    // 렌더타겟 생성
    mShadowRenderTarget = new D3DBitmapRenderTarget(ShadowArea, ShadowArea);

    // Depth Stencil Texture 생성
    D3D11_TEXTURE2D_DESC TexDesc = D3DGraphicsDefault::DefaultTextureDesc;
    TexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    TexDesc.Width = ShadowArea;
    TexDesc.Height = ShadowArea;
    TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    D3DGraphicsTexture2D* pTexture = new D3DGraphicsTexture2D(&TexDesc);

    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
    DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    D3DGraphicsDSV* ShadowDSV = new D3DGraphicsDSV(pTexture, &DSVDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;
    D3DGraphicsSRV* ShadowSRV = new D3DGraphicsSRV(pTexture, &SRVDesc);
    ShadowSRV->SetBindStage(eShaderStage::PS);

    mShadowRenderTarget->PushResourceView(ShadowDSV, ShadowSRV);

    SAFE_RELEASE(pTexture);
}

Light::~Light()
{
    SAFE_RELEASE(mShadowRenderTarget);
    SAFE_RELEASE(mShadowViewport);
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
}

void Light::PreRender()
{
}

void Light::Render()
{
}

void Light::Draw(Camera* _camera)
{
    // 그림자 계산 수행
    if (mShadowViewport && mShadowRenderTarget)
    {
        mShadowViewport->Bind();

        Vector3 eye = _camera->gameObject->transform->GetWorldPosition();
        Vector3 forward = _camera->gameObject->transform->Forward();

        // 그람자의 위치 (카메라 위치 + 카메라 방향만큼의 Distance)
        Vector3 shadowPos = eye + forward * mCameradDist;
        // 광원의 위치 (그림자 위치 + 조명 방향만큼의 Distance)
        Vector3 lightPos = shadowPos - (mLightProp.Direction * mUpDist);

        // Eye = 광원의 위치 LookAt = 그림자의 위치
        // 둘 다 셰이더로 보낼 땐 전치하여 보내야한다.
        mLightProp.ShadowView       = XMMatrixTranspose(XMMatrixLookAtLH(lightPos, shadowPos, Vector3::Up));
        mLightProp.ShadowProjection = XMMatrixTranspose(XMMatrixOrthographicLH(ShadowArea, ShadowArea, mLightNear, mLightFar));

        _camera->PushLight(this);
    }
    Vector3 WolrdPos = gameObject->transform->GetWorldPosition();
    memcpy(&mLightProp.Position, &WolrdPos, sizeof(Vector3)); // 3원소만 복사
    GameManager::GetCurrentWorld()->GetLightSystem()->AddLight(mLightProp);
}

void Light::PostRender()
{
}

void Light::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
}

json Light::Serialize()
{
    json ret;
    
    ret["id"] = GetId();
    json prop = json::object();
    prop["position"] = { mLightProp.Position.x, mLightProp.Position.y, mLightProp.Position.z, mLightProp.Position.w };
    prop["direction"] = { mLightProp.Direction.x, mLightProp.Direction.y, mLightProp.Direction.z, mLightProp.Direction.w };
    prop["radiance"] = { mLightProp.Radiance.r, mLightProp.Radiance.g, mLightProp.Radiance.b, mLightProp.Radiance.a };
    prop["diffuse"] = { mLightProp.DiffuseRGB.r, mLightProp.DiffuseRGB.g, mLightProp.DiffuseRGB.b, mLightProp.DiffuseRGB.a };
    prop["ambient"] = { mLightProp.AmbientRGB.r, mLightProp.AmbientRGB.g, mLightProp.AmbientRGB.b, mLightProp.AmbientRGB.a };
    prop["specular"] = { mLightProp.SpecularRGB.r, mLightProp.SpecularRGB.g, mLightProp.SpecularRGB.b, mLightProp.SpecularRGB.a };
    prop["type"] = mLightProp.LightType;

    ret["property"] = prop;

    return ret;
}

void Light::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    // Ensure the "property" key exists
    if (j.contains("property")) {
        const auto& prop = j["property"];

        // Deserialize position
        if (prop.contains("position")) {
            auto pos = prop["position"];
            if (pos.is_array() && pos.size() == 4) {
                mLightProp.Position.x = pos[0].get<float>();
                mLightProp.Position.y = pos[1].get<float>();
                mLightProp.Position.z = pos[2].get<float>();
                mLightProp.Position.w = pos[3].get<float>();
            }
        }

        // Deserialize direction
        if (prop.contains("direction")) {
            auto dir = prop["direction"];
            if (dir.is_array() && dir.size() == 4) {
                mLightProp.Direction.x = dir[0].get<float>();
                mLightProp.Direction.y = dir[1].get<float>();
                mLightProp.Direction.z = dir[2].get<float>();
                mLightProp.Direction.w = dir[3].get<float>();
            }
        }

        // Deserialize radiance
        if (prop.contains("radiance")) {
            auto rad = prop["radiance"];
            if (rad.is_array() && rad.size() == 4) {
                mLightProp.Radiance.r = rad[0].get<float>();
                mLightProp.Radiance.g = rad[1].get<float>();
                mLightProp.Radiance.b = rad[2].get<float>();
                mLightProp.Radiance.a = rad[3].get<float>();
            }
        }

        // Deserialize diffuse
        if (prop.contains("diffuse")) {
            auto diff = prop["diffuse"];
            if (diff.is_array() && diff.size() == 4) {
                mLightProp.DiffuseRGB.r = diff[0].get<float>();
                mLightProp.DiffuseRGB.g = diff[1].get<float>();
                mLightProp.DiffuseRGB.b = diff[2].get<float>();
                mLightProp.DiffuseRGB.a = diff[3].get<float>();
            }
        }

        // Deserialize ambient
        if (prop.contains("ambient")) {
            auto amb = prop["ambient"];
            if (amb.is_array() && amb.size() == 4) {
                mLightProp.AmbientRGB.r = amb[0].get<float>();
                mLightProp.AmbientRGB.g = amb[1].get<float>();
                mLightProp.AmbientRGB.b = amb[2].get<float>();
                mLightProp.AmbientRGB.a = amb[3].get<float>();
            }
        }

        // Deserialize specular
        if (prop.contains("specular")) {
            auto spec = prop["specular"];
            if (spec.is_array() && spec.size() == 4) {
                mLightProp.SpecularRGB.r = spec[0].get<float>();
                mLightProp.SpecularRGB.g = spec[1].get<float>();
                mLightProp.SpecularRGB.b = spec[2].get<float>();
                mLightProp.SpecularRGB.a = spec[3].get<float>();
            }
        }

        // Deserialize type
        if (prop.contains("type")) {
            mLightProp.LightType = prop["type"].get<int>();
        }
    }
}

void Light::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Light" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::string name = Helper::ToString(gameObject->GetName());
        ImGui::DragFloat(("LightNear" + uid).c_str(), &mLightNear, 1.0f, 1.0f, 100000.0f);
        ImGui::DragFloat(("LightFar" + uid).c_str(), &mLightFar, 1.0f, 1.0f, 100000.0f);
        ImGui::DragFloat(("DistFromCamera" + uid).c_str(), &mCameradDist, 1.0f, 1.0f, 100000.0f);
        ImGui::DragFloat(("UpFromLookAt" + uid).c_str(), &mUpDist, 1.0f, 1.0f, 100000.0f);
        ImGui::NewLine();
        ImGui::DragFloat(("Intensity##" + name).c_str(), &mLightProp.LightIntensity, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat3(("Radiance##" + name).c_str(), &mLightProp.Radiance.r, 0.05f, -1.0f, 1.0f);
        if (mLightProp.LightType == (UINT)eLightType::Direction)
        {
            ImGui::DragFloat3(("Direction##" + name).c_str(), &mLightProp.Direction.x, 0.05f, -1.0f, 1.0f);
        }
        
        
        ImGui::Image((ImTextureID)mShadowRenderTarget->GetSRV(mShadowRenderTarget->GetDSV())->mSRV, ImVec2(200, 200));

        // 래거시렌더링의 잔재.....
        /*ImGui::NewLine();
        ImGui::ColorEdit3(("Diffuse##" + name).c_str(), &mLightProp.DiffuseRGB.r);
        ImGui::NewLine();
        ImGui::ColorEdit3(("Ambient##" + name).c_str(), &mLightProp.AmbientRGB.r);
        ImGui::DragFloat(("AmbientStrength##" + name).c_str(), &mLightProp.AmbientStrength, 0.05f, 0.0f, 1.0f);
        ImGui::NewLine();
        ImGui::ColorEdit3(("Specular##" + name).c_str(), &mLightProp.SpecularRGB.r);*/
    }
}