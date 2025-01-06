#pragma once
#include "Resource/Resource.h"

class VertexShader;
struct InputLayoutDesc;

class InputLayout
    : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::InputLayout);
    explicit InputLayout(std::wstring_view _name, std::vector<InputLayoutDesc>& _pILDesc, VertexShader* _pVertexShader);
    virtual ~InputLayout();
public:
    inline IInputLayout* GetInputLayout() { return mInputLayout; }
private:
    IInputLayout* mInputLayout;
};

