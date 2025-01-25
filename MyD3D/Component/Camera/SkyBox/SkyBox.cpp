#include "pch.h"
#include "SkyBox.h"
#include "Component/Camera/Camera.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

SkyBox* SkyBox::DefaultSkyBox = nullptr;

SkyBox::SkyBox()
{
}

void SkyBox::Draw(Matrix& _view, Matrix& _projection, FLOAT _far)
{
    if (mSkyBoxTexture2D[Environment] && mSkyBoxTexture2D[Diffuse] &&
        mSkyBoxTexture2D[Specular] && mSkyBoxTexture2D[BLDFLookUpTable])
    {
        // 리소스 바인딩
        GraphicsManager::GetRasterizerState(eRasterizerStateType::NONE_CULLING)->Bind();
        GraphicsManager::GetVertexShader(eVertexShaderType::SKYBOX)->Bind();
        GraphicsManager::GetPixelShader(ePixelShaderType::SKYBOX)->Bind();
        mSkyBoxTexture2D[Environment]->Texture->Bind();
        mSkyBoxTexture2D[Diffuse]->Texture->Bind();
        mSkyBoxTexture2D[Specular]->Texture->Bind();
        mSkyBoxTexture2D[BLDFLookUpTable]->Texture->Bind();

        // 속한 월드의 매트릭스가 있으면 나중에 그걸 가져오자
        TransformCBuffer cb;
        float ScaleValue = _far / 2.0f;
        cb.World = Matrix::CreateScale(Vector3(ScaleValue, ScaleValue, ScaleValue));
        cb.View = XMMatrixTranspose(_view);
        cb.Projection = XMMatrixTranspose(_projection);

        GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&cb);

        auto pMesh = MeshResource::GetSkyCubeMesh();
        if (pMesh)
        {
            pMesh->Bind();
            D3DGraphicsRenderer::DrawCall(static_cast<UINT>(pMesh->mIndices.size()), 0, 0);
        }
    }
}

void SkyBox::SetEnvironmentTexture(ResourceHandle _handle)
{
    mSkyBoxTexHandle[Environment] = _handle;
    mSkyBoxTexture2D[Environment] = ResourceManager::GetResource<Texture2DResource>(_handle);
    mSkyBoxTexture2D[Environment]->Texture->SetBindStage(eShaderStage::PS);
    mSkyBoxTexture2D[Environment]->Texture->SetBindSlot(20);
    mSkyBoxTexture2D[Environment]->Texture->Bind();
}

void SkyBox::SetDiffuseTexture(ResourceHandle _handle)
{
    mSkyBoxTexHandle[Diffuse] = _handle;
    mSkyBoxTexture2D[Diffuse] = ResourceManager::GetResource<Texture2DResource>(_handle);
    mSkyBoxTexture2D[Diffuse]->Texture->SetBindStage(eShaderStage::PS);
    mSkyBoxTexture2D[Diffuse]->Texture->SetBindSlot(21);
    mSkyBoxTexture2D[Diffuse]->Texture->Bind();
}

void SkyBox::SetSpecularture(ResourceHandle _handle)
{
    mSkyBoxTexHandle[Specular] = _handle;
    mSkyBoxTexture2D[Specular] = ResourceManager::GetResource<Texture2DResource>(_handle);
    mSkyBoxTexture2D[Specular]->Texture->SetBindStage(eShaderStage::PS);
    mSkyBoxTexture2D[Specular]->Texture->SetBindSlot(22);
    mSkyBoxTexture2D[Specular]->Texture->Bind();
}

void SkyBox::SetLookUpTableTexture(ResourceHandle _handle)
{
    mSkyBoxTexHandle[BLDFLookUpTable] = _handle;
    mSkyBoxTexture2D[BLDFLookUpTable] = ResourceManager::GetResource<Texture2DResource>(_handle);
    mSkyBoxTexture2D[BLDFLookUpTable]->Texture->SetBindStage(eShaderStage::PS);
    mSkyBoxTexture2D[BLDFLookUpTable]->Texture->SetBindSlot(23);
    mSkyBoxTexture2D[BLDFLookUpTable]->Texture->Bind();
}

void SkyBox::EditorRendering(EditorViewerType _viewerType)
{
}

SkyBox* SkyBox::GetDefaultSkyBox()
{
    if (!DefaultSkyBox)
    {
        DefaultSkyBox = new SkyBox;
        ResourceHandle handle[SKYBOX_TEX_SIZE];
        {
            handle[Environment].mResourceType = eResourceType::Texture2DResource;
            handle[Environment].mMainKey = L"Default_SkyBox_EnvironmentTexture";
            handle[Environment].mPath = L"resource/texture/Skybox/DefaultSkyIBLMapEnvHDR.dds";
            DefaultSkyBox->mSkyBoxTexture2D[Environment] = new Texture2DResource(handle[Environment]);
            DefaultSkyBox->mSkyBoxTexture2D[Environment]->Texture->SetBindStage(eShaderStage::PS);
            DefaultSkyBox->mSkyBoxTexture2D[Environment]->Texture->SetBindSlot(20);
        }
        {
            handle[Diffuse].mResourceType = eResourceType::Texture2DResource;
            handle[Diffuse].mMainKey = L"Default_SkyBox_DiffuseTexture";
            handle[Diffuse].mPath = L"resource/texture/Skybox/DefaultSkyIBLMapDiffuseHDR.dds";
            DefaultSkyBox->mSkyBoxTexture2D[Diffuse] = new Texture2DResource(handle[Diffuse]);
            DefaultSkyBox->mSkyBoxTexture2D[Diffuse]->Texture->SetBindStage(eShaderStage::PS);
            DefaultSkyBox->mSkyBoxTexture2D[Diffuse]->Texture->SetBindSlot(21);
        }
        {
            handle[Specular].mResourceType = eResourceType::Texture2DResource;
            handle[Specular].mMainKey = L"Default_SkyBox_SpecularTexture";
            handle[Specular].mPath = L"resource/texture/Skybox/DefaultSkyIBLMapSpecularHDR.dds";
            DefaultSkyBox->mSkyBoxTexture2D[Specular] = new Texture2DResource(handle[Specular]);
            DefaultSkyBox->mSkyBoxTexture2D[Specular]->Texture->SetBindStage(eShaderStage::PS);
            DefaultSkyBox->mSkyBoxTexture2D[Specular]->Texture->SetBindSlot(22);
        }
        {
            handle[BLDFLookUpTable].mResourceType = eResourceType::Texture2DResource;
            handle[BLDFLookUpTable].mMainKey = L"Default_SkyBox_BLDFLookUpTableTexture";
            handle[BLDFLookUpTable].mPath = L"resource/texture/Skybox/DefaultSkyIBLMapBrdf.dds";
            DefaultSkyBox->mSkyBoxTexture2D[BLDFLookUpTable] = new Texture2DResource(handle[BLDFLookUpTable]);
            DefaultSkyBox->mSkyBoxTexture2D[BLDFLookUpTable]->Texture->SetBindStage(eShaderStage::PS);
            DefaultSkyBox->mSkyBoxTexture2D[BLDFLookUpTable]->Texture->SetBindSlot(23);
        }
    }
    return DefaultSkyBox;
}

void SkyBox::FreeDefaultSkyBox()
{
    if (DefaultSkyBox)
    {
        SAFE_DELETE(DefaultSkyBox->mSkyBoxTexture2D[0]);
        SAFE_DELETE(DefaultSkyBox->mSkyBoxTexture2D[1]);
        SAFE_DELETE(DefaultSkyBox->mSkyBoxTexture2D[2]);
        SAFE_DELETE(DefaultSkyBox->mSkyBoxTexture2D[3]);
        SAFE_DELETE(DefaultSkyBox);
    }
}
