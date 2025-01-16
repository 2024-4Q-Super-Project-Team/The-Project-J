#include "pch.h"
#include "SkyBox.h"
#include "Component/Camera/Camera.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

SkyBox::SkyBox(std::wstring_view _name)
    : Resource(_name)
{
}

void SkyBox::Draw(Camera* _camera)
{
    if (mIBLEnvironmentTex && mIBLDiffuseTex &&
        mIBLSpecularTex && mBLDFLookUpTableTex)
    {
        // 리소스 바인딩
        GraphicsManager::GetVertexShader(eVertexShaderType::SKYBOX)->Bind();
        GraphicsManager::GetPixelShader(ePixelShaderType::SKYBOX)->Bind();
        mIBLDiffuseTex->Texture->Bind();
        mIBLDiffuseTex->Texture->Bind();
        mIBLSpecularTex->Texture->Bind();
        mBLDFLookUpTableTex->Texture->Bind();

        // 속한 월드의 매트릭스가 있으면 나중에 그걸 가져오자
        TransformCBuffer cb;
        float ScaleValue = _camera->GetProjectionFar() / 2.0f;
        cb.World = Matrix::CreateScale(Vector3(ScaleValue, ScaleValue, ScaleValue));
        cb.View = XMMatrixTranspose(_camera->GetView());
        cb.Projection = XMMatrixTranspose(_camera->GetProjection());

        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&cb);

        auto pMesh = MeshResource::SkyCubeMesh.get();
        if (pMesh)
        {
            pMesh->Bind();
            D3DGraphicsRenderer::DrawCall(static_cast<UINT>(pMesh->mIndices.size()), 0, 0);
        }
    }
}

void SkyBox::SetEnvironmentTexture(std::shared_ptr<Texture2D> _tex)
{
    mIBLEnvironmentTex = _tex;
    mIBLEnvironmentTex->Texture->SetBindStage(eShaderStage::PS);
    mIBLEnvironmentTex->Texture->SetBindSlot(20);
    mIBLEnvironmentTex->Texture->Bind();
}

void SkyBox::SetDiffuseTexture(std::shared_ptr<Texture2D> _tex)
{
    mIBLDiffuseTex = _tex;
    mIBLDiffuseTex->Texture->SetBindStage(eShaderStage::PS);
    mIBLDiffuseTex->Texture->SetBindSlot(21);
    mIBLDiffuseTex->Texture->Bind();
}

void SkyBox::SetSpecularture(std::shared_ptr<Texture2D> _tex)
{
    mIBLSpecularTex = _tex;
    mIBLSpecularTex->Texture->SetBindStage(eShaderStage::PS);
    mIBLSpecularTex->Texture->SetBindSlot(22);
    mIBLSpecularTex->Texture->Bind();
}

void SkyBox::SetLookUpTableTexture(std::shared_ptr<Texture2D> _tex)
{
    mBLDFLookUpTableTex = _tex;
    mBLDFLookUpTableTex->Texture->SetBindStage(eShaderStage::PS);
    mBLDFLookUpTableTex->Texture->SetBindSlot(23);
    mBLDFLookUpTableTex->Texture->Bind();
}

void SkyBox::EditorRendering()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetName());
	if (ImGui::TreeNodeEx(("SkyBox : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		if (mIBLEnvironmentTex)
		{
            mIBLEnvironmentTex->EditorRendering();
		}
		if (mIBLDiffuseTex)
		{
            mIBLDiffuseTex->EditorRendering();
		}
		if (mIBLSpecularTex)
		{
            mIBLSpecularTex->EditorRendering();
		}
		if (mBLDFLookUpTableTex)
		{
            mBLDFLookUpTableTex->EditorRendering();
		}
		ImGui::TreePop();
	}
}

