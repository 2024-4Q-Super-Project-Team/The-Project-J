#pragma once
#include "Graphics/GraphicsDevice.h"
#include "Graphics/GraphicsRenderer.h"
#include "Graphics/GraphicsRenderTarget.h"

#include "Graphics/Wrapper/GraphicsTexture.h"
#include "Graphics/Wrapper/GraphicsBuffer.h"
#include "Graphics/Wrapper/Shader.h"
#include "Graphics/Wrapper/InputLayout.h"
#include "Graphics/Wrapper/SamplerState.h"

namespace Graphics
{
	class GraphicsDevice;
	class Renderer;
	class RenderTarget;

	HRESULT CreateGraphicsDeviceAndRenderer(GraphicsDevice** _ppGraphicsDevice, Renderer** _ppRenderer);


}