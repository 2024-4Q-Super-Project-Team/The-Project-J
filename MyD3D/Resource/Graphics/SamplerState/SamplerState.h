#pragma once
#include "Resource/Resource.h"

class ISamplerState;

class SamplerState
    : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::SamplerState);
    explicit SamplerState(std::wstring_view _name, SamplerDesc* _pDesc);
    virtual ~SamplerState();
public:
    void Bind();
public:
    IGraphicsRenderer* mRenderer;
    ISamplerState* mSamplerState;
};

