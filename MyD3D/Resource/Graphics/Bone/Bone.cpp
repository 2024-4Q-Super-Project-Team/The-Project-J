#include "pch.h"
#include "Bone.h"

Bone::Bone(std::wstring_view _name, Matrix _matrix)
    : Resource(_name)
    , mOffsetMatrix(_matrix)
{
}

Bone::~Bone()
{
}
