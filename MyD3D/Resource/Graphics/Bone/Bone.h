#pragma once
#include "Resource/Resource.h"

// ���� ������ �޽��� ���ϸ� �޽��κ��� ������� �������ִ��� ����� ������ �ִ�.
class Bone
    : public Resource
{
public:
    RESOURCE_TYPE(Bone);
    explicit Bone(ResourceHandle _handle, Matrix _matrix);
    virtual ~Bone();
public:
    const Matrix  mOffsetMatrix;      // �ش� ���� �ɼ�
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

