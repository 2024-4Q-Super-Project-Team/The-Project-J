#pragma once
#include "Resource/Resource.h"

// ���� ������ �޽��� ���ϸ� �޽��κ��� ������� �������ִ��� ����� ������ �ִ�.
class Bone
{
public:
    explicit Bone(const std::wstring& _key, Matrix& _matrix);
    virtual ~Bone();
public:
    std::wstring mKey;
    const Matrix  mOffsetMatrix;      // �ش� ���� �ɼ�
};