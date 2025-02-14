#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRenderTarget;
class D3DGraphicsViewport;

class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;
// Context를 통한 바인딩 및 Draw를 수행하는 객체 (여야하는데 귀찮아서 Bind는 알아서 하게 만듬)
// Context를 사용하는 모든 함수는 Renderer가 다룸.
class D3DGraphicsRenderer
{
public:
    explicit D3DGraphicsRenderer() = default;
    virtual ~D3DGraphicsRenderer() = default;
public:
    static BOOL Initialize();
    static void Finalization();
public:
    static void UseDepthTest(bool _bValue);
    static void DrawCall(IN UINT _numIndex, IN UINT _startIndex, IN INT _baseVertex);
    static void DrawCall(IN UINT _numIndex, INT _baseVertex);
    static void DrawVertexCall(IN UINT _vertexCount, IN UINT _startVertexLocation);

    static void SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);
public:
    static ID3D11DeviceContext* mDeviceContext;
public:
    static inline ID3D11DeviceContext* GetDevicecontext() { return mDeviceContext; }
public:
    
};

