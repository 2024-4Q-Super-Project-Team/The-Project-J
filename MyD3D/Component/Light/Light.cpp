#include "pch.h"
#include "Light.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "World/WorldManager.h"
#include "Graphics/GraphicsManager.h"

#include "World/World.h"

std::queue<D3DBitmapRenderTarget*>  Light::mShadowRenderTargetPool;

Light::Light(Object* _owner)
    : Component(_owner)
	, mShadowResolution(4096.0f)
    , mShadowDistance(4096.0f)
    , mShadowViewport(nullptr)
    , mShadowRenderTarget(nullptr)
{
    SetEID("Light");
    mType = eComponentType::LIGHT;
    SetLightType(eLightType::Direction);
    mLightProp.Direction = Vector4(0, 0, 1, 0);
    // Viewport 생성
    mShadowViewport = new D3DGraphicsViewport(0.0f, 0.0f, mShadowResolution, mShadowResolution);
    // 렌더타겟 생성
    mShadowRenderTarget = new D3DBitmapRenderTarget(mShadowResolution, mShadowResolution);

    // Depth Stencil Texture 생성
    D3D11_TEXTURE2D_DESC TexDesc = D3DGraphicsDefault::DefaultTextureDesc;
    TexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    TexDesc.Width = mShadowResolution;
    TexDesc.Height = mShadowResolution;
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

    pTexture->mTex->Release();
}

Light::~Light()
{
    SAFE_DELETE(mShadowRenderTarget);
    SAFE_DELETE(mShadowViewport);
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
	// 이걸 한번이라도 안거치면 라이트가 안된다. 초기 view나 Projection이 제대로된 값이 아닌듯?
    if (mLightProp.UseShadow == TRUE && mShadowViewport && mShadowRenderTarget)
    {
        mShadowViewport->Bind();

        Vector3 eye = _camera->gameObject->transform->GetWorldPosition();
        Vector3 forward = _camera->gameObject->transform->Forward();

        // 그람자의 위치 (카메라 위치 + 카메라 방향만큼의 Distance)
        Vector3 shadowPos = eye + forward * mCameraDist;
        // 광원의 위치 (그림자 위치 + 조명 방향만큼의 Distance)
        Vector3 lightPos = Vector3{};
        if (mLightProp.LightType == 0)
        {
            lightPos = shadowPos - (mLightProp.Direction * mUpDist);
        }
        else if (mLightProp.LightType == 1)
        {
            lightPos = gameObject->transform->GetWorldPosition();
        }
        else if (mLightProp.LightType == 2)
        {
            // TODO : 스팟 라이트 그림자 처리?????
        }

        // Eye = 광원의 위치 LookAt = 그림자의 위치
        // 둘 다 셰이더로 보낼 땐 전치하여 보내야한다.
        mLightProp.ShadowView       = XMMatrixTranspose(XMMatrixLookAtLH(lightPos, shadowPos, Vector3::Up));
        mLightProp.ShadowProjection = XMMatrixTranspose(XMMatrixOrthographicLH(mShadowDistance, mShadowDistance, mLightNear, mLightFar));
    }
    Vector3 WolrdPos = gameObject->transform->GetWorldPosition();
    memcpy(&mLightProp.Position, &WolrdPos, sizeof(Vector3)); // 3원소만 복사
    _camera->PushLightList(this);
}

void Light::PostRender()
{
}

void Light::EditorUpdate()
{
    // 그림자 계산 수행
    // 이걸 한번이라도 안거치면 라이트가 안된다. 초기 view나 Projection이 제대로된 값이 아닌듯?
    if (mLightProp.UseShadow == TRUE && mShadowViewport && mShadowRenderTarget)
    {
        mShadowViewport->Bind();

        Vector3 eye = EditorManager::mEditorCamera.mPosition;
        Vector3 forward = EditorManager::mEditorCamera.mDirection;

        // 그람자의 위치 (카메라 위치 + 카메라 방향만큼의 Distance)
        Vector3 shadowPos = eye + forward * mCameraDist;
        // 광원의 위치 (그림자 위치 + 조명 방향만큼의 Distance)
        Vector3 lightPos = shadowPos - (mLightProp.Direction * mUpDist);

        // Eye = 광원의 위치 LookAt = 그림자의 위치
        // 둘 다 셰이더로 보낼 땐 전치하여 보내야한다.
        mLightProp.ShadowView = XMMatrixTranspose(XMMatrixLookAtLH(lightPos, shadowPos, Vector3::Up));
        mLightProp.ShadowProjection = XMMatrixTranspose(XMMatrixOrthographicLH(mShadowDistance, mShadowDistance, mLightNear, mLightFar));
    }
    Vector3 WolrdPos = gameObject->transform->GetWorldPosition();
    memcpy(&mLightProp.Position, &WolrdPos, sizeof(Vector3)); // 3원소만 복사
    EditorManager::mEditorCamera.PushLightList(this);
}

void Light::EditorRender()
{
}

void Light::UpdateLightProperty()
{
}

void Light::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
}

json Light::Serialize()
{
    json ret;
    
    ret["id"] = GetId();
    ret["name"] = "Light";

    //props
    json prop = json::object();
    prop["direction"] = { mLightProp.Direction.x, mLightProp.Direction.y, mLightProp.Direction.z, mLightProp.Direction.w };
    prop["radiance"] = { mLightProp.Radiance.r, mLightProp.Radiance.g, mLightProp.Radiance.b, mLightProp.Radiance.a };
    prop["type"] = mLightProp.LightType;
    prop["strength"] = mLightProp.LightStrengh;
    prop["radius"] = mLightProp.LightRadius;
    prop["cutoff"] = mLightProp.LightCutOff;
    prop["use shadow"] = mLightProp.UseShadow;
    prop["shadow strength"] = mLightProp.ShadowStrengh;
    ret["property"] = prop;

    ret["shadow distance"] = mShadowDistance;
    ret["light near"] = mLightNear;
    ret["light far"] = mLightFar;
    ret["camera dist"] = mCameraDist;
    ret["up dist"] = mUpDist;

    return ret;
}

void Light::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    // Ensure the "property" key exists
    if (j.contains("property")) {
        const auto& prop = j["property"];

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

        // Deserialize type
        if (prop.contains("type")) {
            mLightProp.LightType = prop["type"].get<int>();
        }

        if (prop.contains("strength")) {
            mLightProp.LightStrengh = prop["strength"].get<float>();
        }

        if (prop.contains("radius")) {
            mLightProp.LightRadius = prop["radius"].get<float>();
        }

        if (prop.contains("cutoff")) {
            mLightProp.LightCutOff = prop["cutoff"].get<float>();
        }

        if (prop.contains("use shadow")) {
            mLightProp.UseShadow = (prop["use shadow"].get<int>());
        }

        if (prop.contains("shadow strength")) {
            mLightProp.ShadowStrengh = prop["shadow strength"].get<float>();
        }
    }

    if (j.contains("shadow distance")) {
        mShadowDistance = j["shadow distance"].get<float>();
    }

    if (j.contains("light near")) {
        mLightNear = j["light near"].get<float>();
    }

    if (j.contains("light far")) {
        mLightFar = j["light far"].get<float>();
    }

    if (j.contains("camera dist")) {
        mCameraDist = j["camera dist"].get<float>();
    }
    if (j.contains("up dist")) {
        mUpDist = j["up dist"].get<float>();
    }

}

void Light::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    const char* renderMode[] = { "Direction", "Point", "Spot" };
    int SelectIndex = mLightProp.LightType; // 현재 선택된 항목 (인덱스)

    ImGui::Text("Light Type : ");
    if (ImGui::Combo((uid + "Light Type").c_str(), &SelectIndex, renderMode, IM_ARRAYSIZE(renderMode)))
    {
        SetLightType((eLightType)SelectIndex);
    }

    ImGui::Separator();

    if (mLightProp.LightType == (UINT)eLightType::Direction)
    {
        ImGui::Text("Light Direction : ");
        ImGui::DragFloat3((uid + "Direction").c_str(), &mLightProp.Direction.x, 0.05f, -1.0f, 1.0f);
        mLightProp.Direction.Normalize();
    }

    if (mLightProp.LightType == (UINT)eLightType::Point)
    {
        ImGui::Text("Light Range : ");
        ImGui::DragFloat((uid + "Range").c_str(), &mLightProp.LightRadius, 0.05f, 1.0f, 500.0f);
        ImGui::Text("Light CutOff : ");
        ImGui::DragFloat((uid + "CutOff").c_str(), &mLightProp.LightCutOff, 0.05f, 0.0000001f, 1.0f);
    }

    ImGui::Text("Light Strengh : ");
    ImGui::DragFloat((uid + "LStrengh").c_str(), &mLightProp.LightStrengh, 0.05f, 0.0f, 1.0f);
    ImGui::Text("Light Radiance : ");
    ImGui::DragFloat3((uid + "Radiance").c_str(), &mLightProp.Radiance.r, 0.05f,  0.0f, 1.0f);

    ImGui::Separator();

    ImGui::Checkbox(("Using Shadow" + uid).c_str(), (bool*)&mLightProp.UseShadow);
    ImGui::Text("Shadow Strengh : ");
    ImGui::DragFloat((uid + "SStrengh").c_str(), &mLightProp.ShadowStrengh, 0.05f, 0.0f, 1.0f);
    ImGui::Text("Shadow Distance : ");
    ImGui::DragFloat((uid + "SDistance").c_str(), &mShadowDistance, 1.0f, 1.0f, 10000.0f);

    ImGui::Text("Light Near : ");
    ImGui::DragFloat((uid + "LightNear").c_str(), &mLightNear, 1.0f, 1.0f, 100000.0f);
    ImGui::Text("Light Far : ");
    ImGui::DragFloat((uid + "LightFar").c_str(), &mLightFar, 1.0f, 1.0f, 100000.0f);

    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
    if (ImGui::TreeNodeEx(("Shadow View" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
    {
        ImGui::Image((ImTextureID)mShadowRenderTarget->GetSRV(mShadowRenderTarget->GetDSV())->mSRV, ImVec2(200, 200));
        ImGui::TreePop();
    }
    EDITOR_COLOR_POP(1);
}