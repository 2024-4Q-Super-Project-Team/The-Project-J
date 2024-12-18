#include "pch.h"
#include "GraphicsManager.h"
#include "../Window/IDisplay.h"

using namespace Graphics;

GraphicsManager::GraphicsManager()
{
    Graphics::CreateGraphicsDeviceAndRenderer(&mGraphicsDevice, &mRenderer);
    D3D11_BUFFER_DESC bufDesc = {};
    bufDesc.Usage = D3D11_USAGE_DEFAULT;
    bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufDesc.CPUAccessFlags = 0;

    {
        bufDesc.ByteWidth = sizeof(CTransformBuffer);
        mConstantBuffArr[static_cast<UINT>(eCBufferType::Transform)]
            = new ConstantBuffer(L"CTransformBuffer", mGraphicsDevice, &bufDesc);
    }
    {
        bufDesc.ByteWidth = sizeof(CMaterialBuffer);
        mConstantBuffArr[static_cast<UINT>(eCBufferType::Material)]
            = new ConstantBuffer(L"CMaterialBuffer", mGraphicsDevice, &bufDesc);
    }
    {
        bufDesc.ByteWidth = sizeof(CCameraBuffer);
        mConstantBuffArr[static_cast<UINT>(eCBufferType::Camera)]
            = new ConstantBuffer(L"CCameraBuffer", mGraphicsDevice, &bufDesc);
    }
    {
        bufDesc.ByteWidth = sizeof(CBonePaletteBuffer);
        mConstantBuffArr[static_cast<UINT>(eCBufferType::BonePalette)]
            = new ConstantBuffer(L"CBonePaletteBuffer", mGraphicsDevice, &bufDesc);
    }
}

GraphicsManager::~GraphicsManager()
{
    SAFE_DELETE_ARRAY(mConstantBuffArr)
    SAFE_DELETE_MAP(mRenderTargetTable)
    SAFE_RELEASE(mRenderer)
    SAFE_RELEASE(mGraphicsDevice)
}

BOOL GraphicsManager::Initialize()
{
    return 0;
}

BOOL GraphicsManager::SetUpDisplay(Display::IWindow** _ppWindow)
{
    if (mRenderer)
    {
        Graphics::RenderTarget* pRenderTarget;
        HRESULT hr = mGraphicsDevice->CreateRenderTarget((*_ppWindow)->GetHandle(), &pRenderTarget);
        if (hr == S_OK)
        {
            mRenderTargetTable.insert(std::make_pair((*_ppWindow)->GetParentHandle(), pRenderTarget));
            return TRUE;
        }
    }
    return FALSE;
}
