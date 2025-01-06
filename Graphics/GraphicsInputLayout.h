#pragma once
#include "IGraphics.h"

class GraphicsDevice;
struct InputLayoutDesc;

SET_UTID(_UTID_GRAPHICS_INPUTLAYOUT, eUTID::InputLayout);

class GraphicsInputLayout
    : public IInputLayout
{
public:
    explicit GraphicsInputLayout(const WCHAR* _name, const InputLayoutDesc* _pIADesc
        , UINT _arrSize, IVertexShader* _pVertexShader, GraphicsDevice* _pDevice);
    virtual ~GraphicsInputLayout();
    GraphicsInputLayout(GraphicsInputLayout&) = delete;
    GraphicsInputLayout& operator=(const GraphicsInputLayout&) = delete;
    GraphicsInputLayout(GraphicsInputLayout&&) noexcept = default;
    GraphicsInputLayout& operator=(GraphicsInputLayout&&) noexcept = default;
public:
    virtual UTID    GetTypeID() override;
    virtual HRESULT QueryInterface(const UTID _UTID, void** _ppObject) override;
    virtual void    Release() override;
public:
    virtual const WCHAR* GetName() override;
public:
    const WCHAR* mName;
    ID3D11InputLayout* mInputLayout;
};