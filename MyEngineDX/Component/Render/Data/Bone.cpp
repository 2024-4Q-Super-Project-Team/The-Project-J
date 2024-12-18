#include "pch.h"
#include "Bone.h"

namespace Graphics
{
	BoneResource::BoneResource(std::wstring_view _name, UINT _id, Matrix& _offsetMatrix)
		: IGraphicsResource(_name), mPaletteID(_id), mOffsetMatrix(_offsetMatrix)
	{
	}
	BoneResource::~BoneResource()
	{
	}
	void BoneState::SetBoneMatrix(BoneResource* _pBoneResource, const Matrix& _worldMatrix)
	{
		mBoneMatrixBuffer[_pBoneResource->GetPaletteID()] = XMMatrixTranspose(_pBoneResource->GetBoneMatrix() * _worldMatrix);
	}
}