#include "pch.h"
#include "MeshRenderer.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Texture/Texture.h"

MeshRenderer::MeshRenderer(Object* _owner)
    : RendererComponent(_owner)
    , mMesh(nullptr)
    , mMateiral(new Material)
{
    mType = eComponentType::MESH_RENDERER;
}

MeshRenderer::~MeshRenderer()
{
    SAFE_DELETE(mMateiral)
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Tick()
{
}

void MeshRenderer::FixedUpdate()
{
}

void MeshRenderer::PreUpdate()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::PostUpdate()
{
}

void MeshRenderer::PreRender()
{
}

void MeshRenderer::Render()
{
}

void MeshRenderer::PostRender()
{
}

void MeshRenderer::Draw(Camera* _camera)
{
    if (mMesh)
    {
        mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View = XMMatrixTranspose(_camera->GetView());
        mTransformMatrices.Projection = XMMatrixTranspose(_camera->GetProjection());

        _camera->PushDrawList(this);
    }
}

void MeshRenderer::Clone(Object* _owner, std::unordered_map<std::wstring, Object*> _objTable)
{
    auto clone = _owner->AddComponent<MeshRenderer>();
    // �޽��� �״�� ���ҽ� �����ϸ� �Ǵϱ� �����ͺ���
    clone->mMesh = this->mMesh;
    // ��Ƽ���� ����
    clone->SetMaterial(this->mMateiral->mMaterialResource);
}

void MeshRenderer::DrawMesh(Camera* _camera)
{
    // ��Ƽ���� ���ε�
    if (mMateiral)
    {
        mMateiral->Bind();
    }
    // �޽� ���ε�
    if (mMesh)
    {
        mMesh->Bind();
    }
    mTransformMatrices.World        = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
    mTransformMatrices.View         = XMMatrixTranspose(_camera->GetView());
    mTransformMatrices.Projection   = XMMatrixTranspose(_camera->GetProjection());
    // Ʈ������ ��� ���� ���ε�
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
    D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
}

void MeshRenderer::DrawShadow(Light* _pLight)
{
    // '�޽���' ���ε�
    if (mMesh)
    {
        mMesh->Bind();
    }
    // View, Projection�� �׸����� V,P�� ����Ѵ�.
    mTransformMatrices.World        = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
    mTransformMatrices.View         = _pLight->GetProperty().ShadowView;
    mTransformMatrices.Projection   = _pLight->GetProperty().ShadowProjection;
    // Ʈ������ ��� ���� ���ε�
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
    D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
}

void MeshRenderer::SetMesh(std::shared_ptr<MeshResource> _mesh)
{
    mMesh = _mesh;
}
void MeshRenderer::SetMaterial(std::shared_ptr<MaterialResource> _material)
{
    mMateiral->SetMaterial(_material);
}
std::shared_ptr<MeshResource> MeshRenderer::GetMesh()
{
    return mMesh;
}
Material* MeshRenderer::GetMaterial()
{
    return mMateiral;
}

json MeshRenderer::Serialize()
{
    json ret;
    ret["mesh"] = mMesh->GetName();
    ret["material"] = mMateiral->mMaterialResource->GetName();
    return ret;
}

json MeshRenderer::Deserialize()
{
    return json();
}


#define SetMaterialEditor(typeEnum, label)                                                      \
    if (mMateiral->mMaterialMaps[(UINT)typeEnum])                                               \
    {                                                                                           \
        ImGui::Text(label);                                                                     \
        ImGui::Checkbox(("IsUse##" + std::string(label) + uid).c_str(), (bool*)&UseMap);        \
        mMateiral->mMatCBuffer.SetUsingMap(typeEnum, UseMap);                                   \
        std::wstring wTexPath = mMateiral->mMaterialResource->GetMaterialMapPath(typeEnum);     \
        std::string TexPath = Helper::ToString(wTexPath);                                       \
        ImGui::Text(TexPath.c_str());                                                           \
    }

void MeshRenderer::EditorRendering()
{
    std::string uid = std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::CollapsingHeader(("Mesh Renderer##" + uid).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Mesh       : ");
        ImGui::SameLine(70);
        if (mMesh) {
            std::string meshMame;
            meshMame.assign(mMesh->GetName().begin(),
                mMesh->GetName().end());
            ImGui::Text(meshMame.c_str());
        }
        else
        {
            ImGui::Text("NULL");
        }

        ImGui::Text("Material   : ");
        ImGui::SameLine(70);
        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                MaterialResource* pMat = mMateiral->mMaterialResource.get();
                std::string name = Helper::ToString(pMat->GetName());
                ImGui::Text(name.c_str());

                ImGui::ColorEdit3(("Diffuse##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.DiffuseRGB.r);
                ImGui::ColorEdit3(("Ambient##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.AmbientRGB.r);
                ImGui::ColorEdit3(("Specular##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.SpecularRGB.r);
                ImGui::DragFloat(("Roughness Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.RoughnessScale, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat(("Metallic Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.MetallicScale, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat(("AmbienOcclusion Scale##" + name).c_str(), &mMateiral->mMatCBuffer.MatProp.AmbienOcclusionScale, 0.01f, 0.0f, 1.0f);

                for (int type = 0; type < MATERIAL_MAP_SIZE; ++type)
                {
                    BOOL UseMap = mMateiral->mMatCBuffer.GetUsingMap((eMaterialMapType)type);
                    switch (type)
                    {
                    case (UINT)eMaterialMapType::DIFFUSE:
                        SetMaterialEditor(eMaterialMapType::DIFFUSE, "Diffuse");
                        break;
                    case (UINT)eMaterialMapType::SPECULAR:
                        SetMaterialEditor(eMaterialMapType::SPECULAR, "Specular");
                        break;
                    case (UINT)eMaterialMapType::AMBIENT:
                        SetMaterialEditor(eMaterialMapType::AMBIENT, "Ambient");
                        break;
                    case (UINT)eMaterialMapType::EMISSIVE:
                        SetMaterialEditor(eMaterialMapType::EMISSIVE, "Emissive");
                        break;
                    case (UINT)eMaterialMapType::NORMAL:
                        SetMaterialEditor(eMaterialMapType::NORMAL, "Normal");
                        break;
                    case (UINT)eMaterialMapType::ROUGHNESS:
                        SetMaterialEditor(eMaterialMapType::ROUGHNESS, "Roughness");
                        break;
                    case (UINT)eMaterialMapType::OPACITY:
                        SetMaterialEditor(eMaterialMapType::OPACITY, "Opacity");
                        break;
                    case (UINT)eMaterialMapType::METALNESS:
                        SetMaterialEditor(eMaterialMapType::METALNESS, "Metalness");
                        break;
                    case (UINT)eMaterialMapType::AMBIENT_OCCLUSION:
                        SetMaterialEditor(eMaterialMapType::AMBIENT_OCCLUSION, "AmbienOcclusion");
                        break;
                    default:
                        break;
                    }
                }
                //if (i == (UINT)eMaterialMapType::DIFFUSE)
                //{
                //    if (mMateiral->mMaterialMaps[i])
                //    {
                //        ImGui::Text("Deffuse");
                //        ImGui::Checkbox(("IsUse##Deffuse" + uid).c_str(), (bool*)&mMateiral->mMatCBuffer.UseMap[i]);
                //        /*GraphicsTexture2D* pTex2D;
                //        if (S_OK == mMateiral->mMaterialMaps[i]->GetTexture2D()->QueryInterface(_UTID_GRAPHICS_TEXTURE2D, (void**)&pTex2D))
                //        {
                //            ImVec2 imageSize = ImVec2(512, 512);
                //            ImGui::Image((ImTextureID)pTex2D->GetShaderResourceView(), imageSize);
                //        }*/
                //    }
                //}
            }
            else
            {
                ImGui::Text("NULL");
            }
        }
    }
}
