#include "pch.h"
#include "SkyBox.h"
#include "Component/Camera/Camera.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

SkyBox::SkyBox(World* _pOwnerWorld)
    : mOwnerWorld(_pOwnerWorld)
{
    mSkyBoxVS = ResourceManager::AddResource<VertexShader>(L"resource/shader/SkyBox_VS.cso");
    mSkyBoxPS = ResourceManager::AddResource<PixelShader>(L"resource/shader/SkyBox_PS.cso");
}

void SkyBox::Draw(Camera* _camera)
{
    // �� �־�� Draw����
    if (mSkyBoxVS && mSkyBoxPS)
    {
        if (mIBLEnvironmentTex && mIBLDiffuseTex &&
            mIBLSpecularTex && mBLDFLookUpTableTex)
        {
            mSkyBoxVS->Bind();
            mSkyBoxPS->Bind();
            TransformCBuffer cb;
            // ���� ������ ��Ʈ������ ������ ���߿� �װ� ��������
            float ScaleValue = _camera->GetProjectionFar() / 2.0f;
            cb.World = Matrix::CreateScale(Vector3(ScaleValue, ScaleValue, ScaleValue));
            cb.View = XMMatrixTranspose(_camera->GetView());
            cb.Projection = XMMatrixTranspose(_camera->GetProjection());
            GraphicsManager::UpdateConstantBuffer(eCBufferType::Transform, &cb);

            auto pMesh = MeshResource::SkyCubeMesh.get();
            if (pMesh)
            {
                pMesh->Bind();
                GraphicsManager::GetRenderer()->DrawCall(static_cast<UINT>(pMesh->mIndices.size()), 0, 0);
            }
        }
    }
}

void SkyBox::Bind()
{
    // ���̴� ���ε�
    mSkyBoxVS->Bind();
    mSkyBoxPS->Bind();
    // �ؽ��� ���ε�
    if(mIBLDiffuseTex)
        mIBLDiffuseTex->Bind();
    if (mIBLDiffuseTex)
        mIBLDiffuseTex->Bind();
    if (mIBLSpecularTex)
        mIBLSpecularTex->Bind();
    if (mBLDFLookUpTableTex)
        mBLDFLookUpTableTex->Bind();
}

void SkyBox::SetEnvironmentTexture(std::shared_ptr<Texture2D> _tex)
{
    mIBLEnvironmentTex = _tex;
    mIBLEnvironmentTex->
        SetShaderStage(eShaderStage::PS).
        SetSlot(20);
    mIBLEnvironmentTex->Bind();
}

void SkyBox::SetDiffuseTexture(std::shared_ptr<Texture2D> _tex)
{
    mIBLDiffuseTex = _tex;
    mIBLDiffuseTex->
        SetShaderStage(eShaderStage::PS).
        SetSlot(21);
    mIBLDiffuseTex->Bind();
}

void SkyBox::SetSpecularture(std::shared_ptr<Texture2D> _tex)
{
    mIBLSpecularTex = _tex;
    mIBLSpecularTex->
        SetShaderStage(eShaderStage::PS).
        SetSlot(22);
    mIBLSpecularTex->Bind();
}

void SkyBox::SetLookUpTableTexture(std::shared_ptr<Texture2D> _tex)
{
    mBLDFLookUpTableTex = _tex;
    mBLDFLookUpTableTex->
        SetShaderStage(eShaderStage::PS).
        SetSlot(23);
    mBLDFLookUpTableTex->Bind();
}