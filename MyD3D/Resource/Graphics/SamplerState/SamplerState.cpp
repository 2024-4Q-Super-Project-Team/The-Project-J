#include "pch.h"
#include "SamplerState.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

SamplerState::SamplerState(std::wstring_view _name, SamplerDesc* _pDesc)
    : Resource(_name)
{

    IGraphicsDevice* pDevice = GraphicsManager::GetDevice();
    if (pDevice)
    {
        if (FAILED(pDevice->CreateSamplerState(_name.data(),_pDesc, &mSamplerState)))
        {
            throw std::runtime_error("Hresult Failed to SamplerState::SamplerState->CreateSamplerState()...");
        }
    }
}

SamplerState::~SamplerState()
{
    SAFE_RELEASE(mSamplerState);
}

void SamplerState::Bind()
{
    if (mRenderer)
    {
        mSamplerState->Bind(GraphicsManager::GetRenderer());
    }
}
