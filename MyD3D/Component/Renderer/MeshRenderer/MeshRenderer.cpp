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
    if (isCastShadow)
    {
        // '�޽���' ���ε�
        if (mMesh)
        {
            mMesh->Bind();
        }
        // View, Projection�� �׸����� V,P�� ����Ѵ�.
        mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View = _pLight->GetProperty().ShadowView;
        mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
        // Ʈ������ ��� ���� ���ε�
        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
        D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
    }
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
    ret["id"] = GetId();
    ret["name"] = "MeshRenderer";
    ret["mesh"] = mMesh->GetName();
    ret["material"] = mMateiral->mMaterialResource->GetName();
    return ret;
}

void MeshRenderer::Deserialize(json& j)
{
}


void MeshRenderer::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("Mesh Renderer##" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
        if (mMesh)
        {
            mMesh->EditorRendering();
        }
        else ImGui::Text("NULL Mesh");

        ImGui::Separator();

        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                mMateiral->EditorRendering();
            }
            else ImGui::Text("NULL Material");
        }

        ImGui::Separator();

        EDITOR_COLOR_EXTRA;
        if (ImGui::TreeNodeEx(("Lighting" + uid).c_str(), ImGuiTreeNodeFlags_Selected))
        {
            ImGui::Checkbox(("Rendering Shadows" + uid).c_str(), &isCastShadow);
            ImGui::TreePop();
        }
        EDITOR_COLOR_POP(1);

        ImGui::TreePop();
    }
   
}
