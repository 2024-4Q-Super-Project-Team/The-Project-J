#pragma once

namespace Graphics
{
	class MeshResource;
	class MaterialResource;
	class BoneResource;
	class AnimationResource;
	class ModelNodeResource;
	class ResourceManager;

	class ModelResource : public IGraphicsResource
	{
	public:
		explicit ModelResource(std::wstring_view _name);
		virtual ~ModelResource();
	private:
		std::vector<std::pair<UINT, std::shared_ptr<MeshResource>>> mMeshs;
		std::vector<std::shared_ptr<MaterialResource>> mMaterials;
		std::unordered_map<std::wstring, std::shared_ptr<BoneResource>> mBones;
		std::unordered_map<std::wstring, std::shared_ptr<AnimationResource>> mAnimations;
		std::unordered_map<std::wstring, ModelNodeResource*> mNodes;
	public:
		ModelNodeResource* mRootNode;	
		inline ModelNodeResource*		GetModelNodeResource(const std::wstring& _nodeName);
		inline BoneResource*			GetBoneResource(const std::wstring& _nodeName);
		inline AnimationResource*		GetAnimation(const std::wstring& _nodeName);
	public:
		inline std::vector<std::pair<UINT, std::shared_ptr<MeshResource>>>& GetMeshs() { return mMeshs; }
		inline std::vector<std::shared_ptr<MaterialResource>>& GetMaterials() { return mMaterials; }
		inline std::unordered_map<std::wstring, std::shared_ptr<BoneResource>>& GetBones() { return mBones; }
		inline std::unordered_map<std::wstring, std::shared_ptr<AnimationResource>>& GetAnimations() { return mAnimations; }
		inline std::unordered_map<std::wstring, ModelNodeResource*>& GetNodes() { return mNodes; }
	};

	class ModelNodeResource
	{
	public:
		ModelNodeResource(std::wstring_view _name, ModelNodeResource* _pParent);
		~ModelNodeResource() { for (ModelNodeResource*& child : mChildren) delete child; }
		// 노드 정보
		std::wstring	    		 mNodeName;
		Matrix						 mTransformation = Matrix::Identity;
		ModelNodeResource*				 mParent;
		std::vector<ModelNodeResource*>	 mChildren;
		std::vector<MeshResource*>		 mMeshResources;
		std::vector<MaterialResource*>	 mMaterials;
	};
}


