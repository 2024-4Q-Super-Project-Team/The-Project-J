#include "pch.h"
#include "Renderer.h"

RendererComponent::RendererComponent(Object* _owner)
    : Component(_owner)
{
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::UpdateOffsetMatrix()
{
    Matrix tr = DirectX::XMMatrixTranslationFromVector(mOffsetPosition);
    Matrix ro = DirectX::XMMatrixRotationQuaternion(mOffsetRotation);
    Matrix sc = DirectX::XMMatrixScalingFromVector(mOffsetScale);

    mOffsetMatrix = sc * ro * tr;
}
