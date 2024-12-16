#include "pch.h"
#include "Model.h"

namespace Graphics
{
	ModelInfo::ModelInfo(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	ModelInfo::~ModelInfo()
	{
	}
	inline ModelNodeInfo* ModelInfo::GetModelNodeInfo(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mNodes);
		return *find;
	}
	inline BoneInfo* ModelInfo::GetBoneInfo(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mBones);
		return find->get();
	}
	inline AnimationInfo* ModelInfo::GetAnimation(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mAnimations);
		return find->get();
	}
	ModelNodeInfo::ModelNodeInfo(std::wstring_view _name, ModelNodeInfo* _pParent)
		: mParent(_pParent), mNodeName(_name)
	{
		if (mParent)
		{
			mParent->mChildren.push_back(this);
		}
	}
}
