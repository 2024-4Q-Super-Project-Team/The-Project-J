#include "pch.h"
#include "MeshRenderer.h"
#include "Object/Object.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Texture/Texture.h"
// Editor
#include "Editor/Handler/EditorDragNDrop.h"

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
    // 메쉬는 그대로 리소스 참조하면 되니까 포인터복사
    clone->mMesh = this->mMesh;
    // 머티리얼 생성
    clone->SetMaterial(this->mMateiral->mMaterialResource);
}

void MeshRenderer::DrawMesh(Camera* _camera)
{
    // 머티리얼 바인딩
    if (mMateiral)
    {
        mMateiral->Bind();
    }
    // 메쉬 바인딩
    if (mMesh)
    {
        mMesh->Bind();
    }
    mTransformMatrices.World        = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
    mTransformMatrices.View         = XMMatrixTranspose(_camera->GetView());
    mTransformMatrices.Projection   = XMMatrixTranspose(_camera->GetProjection());
    // 트랜스폼 상수 버퍼 바인딩
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&mTransformMatrices);
    D3DGraphicsRenderer::DrawCall(static_cast<UINT>(mMesh->mIndices.size()), 0, 0);
}

void MeshRenderer::DrawShadow(Light* _pLight)
{
    if (isCastShadow)
    {
        // '메쉬만' 바인딩
        if (mMesh)
        {
            mMesh->Bind();
        }
        // View, Projection은 그림자의 V,P로 써야한다.
        mTransformMatrices.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
        mTransformMatrices.View = _pLight->GetProperty().ShadowView;
        mTransformMatrices.Projection = _pLight->GetProperty().ShadowProjection;
        // 트랜스폼 상수 버퍼 바인딩
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
    if (mMesh) 
        ret["mesh"] = Helper::ToString(mMesh->GetName());
    else 
        ret["mesh"] = nullptr;

    if (mMateiral && mMateiral->mMaterialResource) 
        ret["material"] = mMateiral->mMaterialResource->GetName();
    else  
        ret["material"] = nullptr;
    return ret;
}

void MeshRenderer::Deserialize(json& j)
{
}


void MeshRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("Mesh Renderer##" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
        {
            std::string uid = "NULL Mesh";
            std::string name = "NULL Mesh";
            if (mMesh)
            {
                mMesh->EditorRendering(EditorViewerType::DEFAULT);
                name = Helper::ToString(mMesh->GetName());
                uid = mMesh->GetID();
            }
            else
            {
                EDITOR_COLOR_NULL;
                ImGui::Selectable(uid.c_str() , false, ImGuiSelectableFlags_Highlight);
                EDITOR_COLOR_POP(1);
            }
            EditorDragNDrop::ReceiveDragAndDropResourceData(uid.c_str(), mMesh);
        }

        ImGui::Separator();

        if (mMateiral)
        {
            if (mMateiral->mMaterialResource)
            {
                mMateiral->EditorRendering(EditorViewerType::DEFAULT);
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
