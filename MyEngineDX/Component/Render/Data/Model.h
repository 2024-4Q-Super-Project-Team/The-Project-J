#pragma once

namespace Graphics
{
	class MeshInfo;
	class MaterialInfo;
	class BoneInfo;
	class AnimationInfo;
	class ModelNodeInfo;
	class ResourceManager;

	class ModelInfo : public IGraphicsResource
	{
	public:
		explicit ModelInfo(std::wstring_view _name);
		virtual ~ModelInfo();
	private:
		std::vector<std::pair<UINT, std::shared_ptr<MeshInfo>>> mMeshs;
		std::vector<std::shared_ptr<MaterialInfo>> mMaterials;
		std::unordered_map<std::wstring, std::shared_ptr<BoneInfo>> mBones;
		std::unordered_map<std::wstring, std::shared_ptr<AnimationInfo>> mAnimations;
		std::unordered_map<std::wstring, ModelNodeInfo*> mNodes;
	public:
		ModelNodeInfo* mRootNode;	
		inline ModelNodeInfo*		GetModelNodeInfo(const std::wstring& _nodeName);
		inline BoneInfo*			GetBoneInfo(const std::wstring& _nodeName);
		inline AnimationInfo*		GetAnimation(const std::wstring& _nodeName);
	public:
		inline std::vector<std::pair<UINT, std::shared_ptr<MeshInfo>>>& GetMeshs() { return mMeshs; }
		inline std::vector<std::shared_ptr<MaterialInfo>>& GetMaterials() { return mMaterials; }
		inline std::unordered_map<std::wstring, std::shared_ptr<BoneInfo>>& GetBones() { return mBones; }
		inline std::unordered_map<std::wstring, std::shared_ptr<AnimationInfo>>& GetAnimations() { return mAnimations; }
		inline std::unordered_map<std::wstring, ModelNodeInfo*>& GetNodes() { return mNodes; }
	};

	class ModelNodeInfo
	{
	public:
		ModelNodeInfo(std::wstring_view _name, ModelNodeInfo* _pParent);
		~ModelNodeInfo() { for (ModelNodeInfo*& child : mChildren) delete child; }
		// 노드 정보
		std::wstring	    		 mNodeName;
		Matrix						 mTransformation = Matrix::Identity;
		ModelNodeInfo*				 mParent;
		std::vector<ModelNodeInfo*>	 mChildren;
		std::vector<MeshInfo*>		 mMeshInfos;
		std::vector<MaterialInfo*>	 mMaterials;
	};
}


