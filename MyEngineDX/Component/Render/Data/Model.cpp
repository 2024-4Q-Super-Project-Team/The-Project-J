#include "pch.h"
#include "Model.h"

namespace Graphics
{
	ModelResource::ModelResource(std::wstring_view _name)
		: IGraphicsResource(_name)
	{
	}
	ModelResource::~ModelResource()
	{
	}
	inline ModelNodeResource* ModelResource::GetModelNodeResource(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mNodes);
		return *find;
	}
	inline BoneResource* ModelResource::GetBoneResource(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mBones);
		return find->get();
	}
	inline AnimationResource* ModelResource::GetAnimation(const std::wstring& _nodeName)
	{
		auto find = Helper::FindMap(_nodeName, mAnimations);
		return find->get();
	}
	ModelNodeResource::ModelNodeResource(std::wstring_view _name, ModelNodeResource* _pParent)
		: mParent(_pParent), mNodeName(_name)
	{
		if (mParent)
		{
			mParent->mChildren.push_back(this);
		}
	}
}
