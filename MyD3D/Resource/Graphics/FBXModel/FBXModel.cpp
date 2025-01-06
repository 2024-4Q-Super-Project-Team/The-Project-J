#include "pch.h"
#include "FBXModel.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"

FBXModelResource::FBXModelResource(std::wstring_view _name)
    : Resource(_name)
    , mRootNode(nullptr)
{
    auto pModel = FBXImporter::ImportFBXModel_All(_name.data());
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
    for (auto& rsc : mMaterialArray)
        ResourceManager::PushResource(rsc);
    for (auto& rsc : mMeshArray)
        ResourceManager::PushResource(rsc);
    for (auto& rsc : mAnimationArray)
        ResourceManager::PushResource(rsc);
}

FBXModelResource::~FBXModelResource()
{
    // 노드와 본은 이 모델말고 안쓰니까 삭제하자
    SAFE_DELETE_ARRAY(mBoneArray);
    SAFE_DELETE_ARRAY(mModelNodeArray);
}

ModelNode::ModelNode(std::wstring_view _name, ModelNode* _pParent)
    : mNodeName(_name), mParent(_pParent)
{
    if (_pParent)
    {
        _pParent->mChildren.push_back(this);
    }
}
