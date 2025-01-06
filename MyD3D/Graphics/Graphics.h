#pragma once

class GraphicsResource;

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
    static BOOL Initialize() {}
    static void Finalize() {}
public:
    void UpdateGraphicsResource(GraphicsResource* _pResource);
private:
    ID3D11Device*        mDevice;
    IDXGIFactory*        mFactory;
    IDXGIAdapter3*       mAdapter;
    ID3D11DeviceContext* mContext;
private:

};

