#include "pch.h"
#include "Light.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Light/LightSystem.h"

Light::Light(Object* _owner)
    : Component(_owner)
{
    mType = eComponentType::LIGHT;
    SetLightType(eLightType::Direction);
    mLightProp.Direction = Vector4(0, 0, 1, 0);
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
}

void Light::PreRender()
{
    GameManager::GetCurrentWorld()->GetLightSystem()->AddLight(mLightProp);
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
