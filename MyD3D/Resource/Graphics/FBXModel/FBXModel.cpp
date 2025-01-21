#include "pch.h"
#include "FBXModel.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"

#include "Resource/Graphics/Material/Material.h"

FBXModelResource::FBXModelResource(ResourceHandle _handle)
    : Resource(_handle)
    , mRootNode(nullptr)
{
    auto pModel = FBXImporter::ImportFBXModel_All(_handle);
    mMaterialArray   = std::move(pModel->MaterialArray);
    mMaterialTable   = std::move(pModel->MaterialTable);
    mMeshArray       = std::move(pModel->MeshArray);
    mMeshTable       = std::move(pModel->MeshTable);
    mAnimationArray  = std::move(pModel->AnimationArray);
    mAnimationTable  = std::move(pModel->AnimationTable);
    mBoneArray       = std::move(pModel->BoneArray);
    mBoneTable       = std::move(pModel->BoneTable);
    mModelNodeArray  = std::move(pModel->ModelNodeArray);
    mModelNodeTable  = std::move(pModel->ModelNodeTable);
    mRootNode       = pModel->RootNode;
    pModel->Clear();

	for (auto& resource : mMaterialArray)
	{
		ResourceManager::PushResource(resource);
	}
	for (auto& resource : mMeshArray)
	{
		ResourceManager::PushResource(resource);
	}
	for (auto& resource : mAnimationArray)
	{
		ResourceManager::PushResource(resource);
	}

	// Path 추가하기
	ResourceHandle handle = { eResourceType::PrefabResource, GetKey() + L"_Prefab", L"", L""};
	mModelPrefab = new PrefabResource(handle, this);
	ResourceManager::PushResource(mModelPrefab);
}

FBXModelResource::~FBXModelResource()
{
    // 노드와 본은 이 모델말고 안쓰니까 삭제하자
    SAFE_DELETE_ARRAY(mBoneArray);
    SAFE_DELETE_ARRAY(mModelNodeArray);
}

void FBXModelResource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(GetKey());

	if (ImGui::TreeNodeEx(("FBXModel" + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		if (ImGui::TreeNodeEx("Material", EDITOR_FLAG_RESOURCE))
		{
			for (auto& rsc : mMaterialArray)
			{
				if (rsc)
					rsc->EditorRendering(EditorViewerType::DEFAULT);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Mesh", EDITOR_FLAG_RESOURCE))
		{
			for (auto& rsc : mMeshArray)
			{
				if (rsc)
					rsc->EditorRendering(EditorViewerType::DEFAULT);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Animation", EDITOR_FLAG_RESOURCE))
		{
			for (auto& rsc : mAnimationArray)
			{
				if (rsc)
					rsc->EditorRendering(EditorViewerType::DEFAULT);
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

ModelNode::ModelNode(std::wstring_view _name, ModelNode* _pParent)
    : mNodeName(_name), mParent(_pParent)
{
    if (_pParent)
    {
        _pParent->mChildren.push_back(this);
    }
}
