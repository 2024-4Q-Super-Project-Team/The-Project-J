#pragma once
//Direct3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>
#include <Directxtk/PrimitiveBatch.h>
#include <Directxtk/VertexTypes.h>
#include <DirectXTex.h>
#include <Psapi.h>
using namespace DirectX;
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

enum class eShaderStage
{
    PASS,   // PASS
    VS,		// Vertex Shader
    PS,		// Pixel Shader
    GS,		// Geometry Shader
    HS,		// Hull Shader
    DS,		// Domain Shader
    CS,		// Compute Shader
    ALL,
};

enum class eUTID
{
    Error,
    Unknown,
    GraphicsDevice,
    GraphicsRenderer,
    GraphicsRenderTarget,

    GraphicsResource,

    Texture2D,
    Texture3D,

    Buffer,
    VertexBuffer,
    IndexBuffer,
    ConstantBuffer,

    InputLayout,
    Shader,
    VertexShader,
    PixelShader,

    SamplerState,
};
typedef unsigned int UTID;
// unique type identifiers
#define SET_UTID(ID_VALUE, TYPE) constexpr static const UTID ID_VALUE = static_cast<UTID>(TYPE)

#include "IGraphics.h"
#include "GraphicsDesc.h"

HRESULT CreateGraphicsDeviceAndRenderer(IGraphicsDevice** _ppGraphicsDevice, IGraphicsRenderer** _ppRenderer);

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
