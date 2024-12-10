#pragma once
#include "Graphics/GraphicsEnum.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/GraphicsRenderer.h"
#include "Graphics/GraphicsRenderTarget.h"
#include "Graphics/GraphicsManager.h"

namespace Graphics
{
	class GraphicsDevice;
	class Renderer;
	class RenderTarget;

	HRESULT CreateGraphicsDeviceAndRenderer(GraphicsDevice** _ppGraphicsDevice, Renderer** _ppRenderer);
}