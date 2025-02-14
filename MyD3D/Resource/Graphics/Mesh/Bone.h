#pragma once
#include "Resource/Resource.h"

// 본은 각각의 메쉬에 속하며 메쉬로부터 어느정도 떨어져있는지 행렬을 가지고 있다.
class Bone
{
public:
    explicit Bone(const std::wstring& _key, Matrix& _matrix);
    virtual ~Bone();
public:
    std::wstring mKey;
    const Matrix  mOffsetMatrix;      // 해당 본의 옵셋
};