#pragma once

class D3DGraphicsDevice;

class IGraphics
{
public:
    virtual void Release() = 0;
};

class I3DGraphics : public IGraphics
{

};

// 메인 GPU장치 리소스
class I3DGraphicsCore : public I3DGraphics
{

};

// GPU 바인딩 리소스
class I3DGraphicsGPUResource : public I3DGraphics
{
public:
    virtual HRESULT Create() = 0;
    virtual HRESULT Bind() = 0;
    virtual HRESULT Reset() = 0;
};

class GraphicsResourceKeyHandler
{
public:
    explicit GraphicsResourceKeyHandler(std::wstring_view _name) : mName(_name) {}
    virtual ~GraphicsResourceKeyHandler() = default;
public:
    inline const std::wstring& GetName() { return mName; }
protected:
    std::wstring mName;
};

class RenderStageBindHandler
{
public:
    inline void SetBindStage(eShaderStage _stage) { mStage = _stage; }
    inline eShaderStage GetBindStage() { return mStage; }
protected:
    eShaderStage mStage = eShaderStage::PASS;
};

class RegisterSlotBindHandler
{
public:
    inline void SetBindSlot(UINT _slot) { mSlot = _slot; }
    inline UINT GetBindSlot() { return mSlot; }
protected:
    UINT mSlot = 0;
};

class DataReadSTDBindHandler
{
public:
    inline void SetStride(UINT _stride) { mStride = _stride; }
    inline void SetOffset(UINT _offset) { mOffset = _offset; }
    inline void SetFormat(DXGI_FORMAT _format) { mFormat = _format; }
    inline UINT GetStride() { return mStride; }
    inline UINT GetOffset() { return mOffset; }
    inline UINT GetFormat() { return mFormat; }
protected:
    UINT mStride;
    UINT mOffset;
    DXGI_FORMAT mFormat;
};

class UpdateGPUResourceHandler
{
public:
    virtual void UpdateGPUResoure(IN const void* _srcData) = 0;
};