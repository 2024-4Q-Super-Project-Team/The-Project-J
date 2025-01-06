#pragma once
#include "IGraphics.h"

class GraphicsDevice;

SET_UTID(_UTID_GRAPHICS_TEXTURE2D, eUTID::Texture2D);
SET_UTID(_UTID_GRAPHICS_TEXTURE3D, eUTID::Texture3D);

class GraphicsTexture2D
	: public ITexture2D
{
public:
	explicit GraphicsTexture2D(const std::wstring& _path, GraphicsDevice* _pDevice);
	virtual ~GraphicsTexture2D();
	GraphicsTexture2D(GraphicsTexture2D&) = delete;
	GraphicsTexture2D& operator=(const GraphicsTexture2D&) = delete;
	GraphicsTexture2D(GraphicsTexture2D&&) noexcept = default;
	GraphicsTexture2D& operator=(GraphicsTexture2D&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
	virtual const WCHAR* GetName() override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    virtual ITexture& SetSlot(UINT _startSlot) override;
    virtual ITexture& SetShaderStage(eShaderStage _stage) override;
public:
    inline ID3D11ShaderResourceView* GetShaderResourceView() const { return mTexRV; }
private:
    const WCHAR* mName;
    UINT         mSlot;
    eShaderStage mShaderStage;
	ID3D11ShaderResourceView* mTexRV;
};

// ¹Ì±¸Çö
class GraphicsTexture3D
	: public ITexture3D
{
public:
	explicit GraphicsTexture3D(GraphicsDevice* _pDevice, const std::wstring& _path);
	virtual ~GraphicsTexture3D();
	GraphicsTexture3D(GraphicsTexture3D&) = delete;
	GraphicsTexture3D& operator=(const GraphicsTexture3D&) = delete;
	GraphicsTexture3D(GraphicsTexture3D&&) noexcept = default;
	GraphicsTexture3D& operator=(GraphicsTexture3D&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
	virtual const WCHAR* GetName() override;
public:
    virtual HRESULT Bind(IGraphicsRenderer* _renderer) override;
    virtual HRESULT Reset(IGraphicsRenderer* _renderer) override;
public:
    virtual ITexture& SetSlot(UINT _startSlot) override;
    virtual ITexture& SetShaderStage(eShaderStage _stage) override;
private:
	const WCHAR* mName;
    UINT         mSlot;
    eShaderStage mShaderStage;
	ID3D11ShaderResourceView* mTexRV;
};

