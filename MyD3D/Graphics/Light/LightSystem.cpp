#include "pch.h"
#include "LightSystem.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

void LightSystem::Bind()
{
    GraphicsManager::UpdateConstantBuffer(eCBufferType::Light, &mLightCBuffer);
    mLightCBuffer.NumLight = 0;
}

void LightSystem::AddLight(LightProperty& _lightProp)
{
    if (mLightCBuffer.NumLight < NUM_MAX_LIGHTS)
    {
        mLightCBuffer.LightProp[mLightCBuffer.NumLight] = _lightProp;
        mLightCBuffer.NumLight++;
    }
}