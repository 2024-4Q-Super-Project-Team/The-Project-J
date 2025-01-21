#include "pch.h"
#include "Bone.h"

Bone::Bone(const std::wstring& _key, Matrix& _matrix)
    : mKey(_key),  mOffsetMatrix(_matrix)
{
}

Bone::~Bone()
{
}
