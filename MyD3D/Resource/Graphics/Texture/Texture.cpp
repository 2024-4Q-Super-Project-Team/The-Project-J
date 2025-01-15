#include "pch.h"
#include "Texture.h"
#include "Manager/GameManager.h"
#include "Graphics/GraphicsManager.h"

Texture2D::Texture2D(std::wstring_view _path)
    : Resource(_path)
    , Texture(new D3DGraphicsImg(_path))
{
}

Texture2D::~Texture2D()
{
    SAFE_RELEASE(Texture);
}

void Texture2D::EditorRendering()
{
}
