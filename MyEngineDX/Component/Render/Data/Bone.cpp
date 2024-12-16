#include "pch.h"
#include "Bone.h"

namespace Graphics
{
	BoneInfo::BoneInfo(std::wstring_view _name, UINT _id, Matrix& _offsetMatrix)
		: IGraphicsResource(_name), mPaletteID(_id), mOffsetMatrix(_offsetMatrix)
	{
	}
	BoneInfo::~BoneInfo()
	{
	}
	void BoneState::SetBoneMatrix(BoneInfo* _pBoneInfo, const Matrix& _worldMatrix)
	{
		mBoneMatrixBuffer[_pBoneInfo->GetPaletteID()] = XMMatrixTranspose(_pBoneInfo->GetBoneMatrix() * _worldMatrix);
	}
}