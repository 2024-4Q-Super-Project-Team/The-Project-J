#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Interface/IGraphics.h"

// 다중 뷰포트에 대한 처리도 생각해봐야 한다
class D3DGraphicsViewport
    : public I3DGraphicsGPUResource
{
public:
    explicit D3DGraphicsViewport(FLOAT _offsetX, FLOAT _offsetY,
        FLOAT _width, FLOAT _height, FLOAT _minDepth = 0.0f, FLOAT _maxDepth = 1.0f);
    virtual ~D3DGraphicsViewport() = default;
public:
    virtual void Release() override;
    virtual HRESULT Create() override;
    virtual HRESULT Bind() override;
    virtual HRESULT Reset() override;
public:
    inline void SetWidth(FLOAT _width) { mViewport.Width = _width; }
    inline void SetHeight(FLOAT _height) { mViewport.Height = _height; }
    inline void SetOffsetX(FLOAT _offsetX) { mViewport.TopLeftX = _offsetX; }
    inline void SetOffsetY(FLOAT _offsetY) { mViewport.TopLeftY = _offsetY; }

    inline FLOAT GetWidth() { return mViewport.Width; }
    inline FLOAT GetHeight() { return mViewport.Height; }
    inline FLOAT GetOffsetX() { return mViewport.TopLeftX; }
    inline FLOAT GetOffsetY() { return mViewport.TopLeftY; }
private:
    D3D11_VIEWPORT mViewport;
};