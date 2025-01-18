#pragma once
#include "Resource/Resource.h"

// 본은 각각의 메쉬에 속하며 메쉬로부터 어느정도 떨어져있는지 행렬을 가지고 있다.
class Bone
    : public Resource
{
public:
    RESOURCE_TYPE(Bone);
    explicit Bone(std::wstring_view _name, Matrix _matrix);
    virtual ~Bone();
public:
    const Matrix  mOffsetMatrix;      // 해당 본의 옵셋
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

