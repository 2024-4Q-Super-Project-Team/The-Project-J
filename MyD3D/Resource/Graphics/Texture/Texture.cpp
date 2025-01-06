#include "pch.h"
#include "Texture.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

Texture2D::Texture2D(std::wstring_view _path)
    : Resource(_path)
{
    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateTexture2D(_path.data(), &mTexture2D)))
        {
            throw std::runtime_error("Hresult Failed to Texture2D::Texture2D->CreateTexture2D()...");
        }
    }
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind()
{
    auto pRenderer = GraphicsManager::GetRenderer();
    if (!pRenderer) return;
    mTexture2D->Bind(pRenderer);
}

void Texture2D::Reset()
{
    auto pRenderer = GraphicsManager::GetRenderer();
    if (!pRenderer) return;
    mTexture2D->Reset(pRenderer);
}

Texture2D& Texture2D::SetShaderStage(eShaderStage _stage)
{
    mTexture2D->SetShaderStage(_stage);
    return *this;
}

Texture2D& Texture2D::SetSlot(UINT _slot)
{
    mTexture2D->SetSlot(_slot);
    return *this;
}
