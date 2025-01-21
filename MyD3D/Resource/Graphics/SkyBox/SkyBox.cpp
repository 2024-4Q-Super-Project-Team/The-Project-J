#include "pch.h"
#include "SkyBox.h"
#include "Component/Camera/Camera.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

std::shared_ptr<SkyBox> SkyBox::DefaultSkyBox;

SkyBox::SkyBox(ResourceHandle _handle)
    : Resource(_handle)
{
}

void SkyBox::Draw(Matrix& _view, Matrix& _projection, FLOAT _far)
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
        float ScaleValue = _far / 2.0f;
        cb.World = Matrix::CreateScale(Vector3(ScaleValue, ScaleValue, ScaleValue));
        cb.View = XMMatrixTranspose(_view);
        cb.Projection = XMMatrixTranspose(_projection);

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

void SkyBox::InitSkyBoxResource()
{
    ResourceHandle handle = { eResourceType::SkyBox, L"Default_SkyBox", L"", L"" };
    DefaultSkyBox = std::make_shared<SkyBox>(handle);
    {
        ResourceHandle texHandle = {
            eResourceType::Texture2D,
            L"Default_SkyBox_Environment_Texture",
            L"",
            L"resource/texture/Skybox/DefaultSkyIBLMapEnvHDR.dds" 
        };
        DefaultSkyBox->SetEnvironmentTexture(std::make_shared<Texture2D>(texHandle));
    }
    {
        ResourceHandle texHandle = {
            eResourceType::Texture2D,
            L"Default_SkyBox_Diffuse_Texture",
            L"",
            L"resource/texture/Skybox/DefaultSkyIBLMapDiffuseHDR.dds"
        };
        DefaultSkyBox->SetDiffuseTexture(std::make_shared<Texture2D>(texHandle));
    }
    {
        ResourceHandle texHandle = {
            eResourceType::Texture2D,
            L"Default_SkyBox_Specular_Texture",
            L"",
            L"resource/texture/Skybox/DefaultSkyIBLMapSpecularHDR.dds"
        };
        DefaultSkyBox->SetSpecularture(std::make_shared<Texture2D>(texHandle));
    }
    {
        ResourceHandle texHandle = {
            eResourceType::Texture2D,
            L"Default_SkyBox_LUT_Texture",
            L"",
            L"resource/texture/Skybox/DefaultSkyIBLMapBrdf.dds"
        };
        DefaultSkyBox->SetLookUpTableTexture(std::make_shared<Texture2D>(texHandle));
    }
}

void SkyBox::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetKey());
	if (ImGui::TreeNodeEx(("SkyBox : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		if (mIBLEnvironmentTex)
		{
            mIBLEnvironmentTex->EditorRendering(EditorViewerType::DEFAULT);
		}
		if (mIBLDiffuseTex)
		{
            mIBLDiffuseTex->EditorRendering(EditorViewerType::DEFAULT);
		}
		if (mIBLSpecularTex)
		{
            mIBLSpecularTex->EditorRendering(EditorViewerType::DEFAULT);
		}
		if (mBLDFLookUpTableTex)
		{
            mBLDFLookUpTableTex->EditorRendering(EditorViewerType::DEFAULT);
		}
		ImGui::TreePop();
	}
}

