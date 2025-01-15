#pragma once
#include "Resource/Resource.h"

// ���� ������ �޽��� ���ϸ� �޽��κ��� ������� �������ִ��� ����� ������ �ִ�.
class Bone
    : public Resource
	, public IEditorObject
{
public:
    RESOURCE_TYPE(eResourceType::Bone);
    explicit Bone(std::wstring_view _name, Matrix _matrix);
    virtual ~Bone();
public:
    const Matrix  mOffsetMatrix;      // �ش� ���� �ɼ�
public:
	virtual void EditorRendering() override;
};

