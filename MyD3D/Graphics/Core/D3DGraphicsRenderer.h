#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

class D3DGraphicsRenderTarget;
class D3DGraphicsViewport;

class D3DGraphicsVertexBuffer;
class D3DGraphicsIndexBuffer;
// Context�� ���� ���ε� �� Draw�� �����ϴ� ��ü (�����ϴµ� �����Ƽ� Bind�� �˾Ƽ� �ϰ� ����)
// Context�� ����ϴ� ��� �Լ��� Renderer�� �ٷ�.
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

