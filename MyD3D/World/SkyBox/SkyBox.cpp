#include "pch.h"
#include "SkyBox.h"
#include "Component/Camera/Camera.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

SkyBox::SkyBox(World* _pOwnerWorld)
    : mOwnerWorld(_pOwnerWorld)
{
    mSkyBoxVS = GraphicsManager::GetVertexShader(eVertexShaderType::SKYBOX);
    mSkyBoxPS = GraphicsManager::GetPixelShader(ePixelShaderType::SKYBOX);
}

void SkyBox::Draw(Camera* _camera)
{
    // 다 있어야 Draw가능
    if (mSkyBoxVS && mSkyBoxPS)
    {
        if (mIBLEnvironmentTex && mIBLDiffuseTex &&
            mIBLSpecularTex && mBLDFLookUpTableTex)
        {
            mSkyBoxVS->Bind();
            mSkyBoxPS->Bind();

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
}

void SkyBox::Bind()
{
    // 셰이더 바인딩
    mSkyBoxVS->Bind();
    mSkyBoxPS->Bind();
    // 텍스쳐 바인딩
    if (mIBLDiffuseTex)
        mIBLDiffuseTex->Texture->Bind();
    if (mIBLDiffuseTex)
        mIBLDiffuseTex->Texture->Bind();
    if (mIBLSpecularTex)
        mIBLSpecularTex->Texture->Bind();
    if (mBLDFLookUpTableTex)
        mBLDFLookUpTableTex->Texture->Bind();
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

