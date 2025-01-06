#pragma once
#include "Resource/Resource.h"

class MeshResource;
class MaterialResource;
class AnimationResource;
class Bone;
class ModelNode;
class Prefab;

// 걍 얘는 신창섭임. 모델의 모든 정보를 가지고 있다 보면 됨. ㅇㅇ
// 유니티의 아바타 같은 느낌이라 생각하면 될듯
class FBXModelResource : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::FBXModel);
    explicit FBXModelResource(std::wstring_view _name);
    virtual ~FBXModelResource();
public:
    // 이걸 굳이 다 shared_ptr로? 할필요가 있을까 ㅇㅇ
    // Material
    std::vector<std::shared_ptr<MaterialResource>> mMaterialArray;
    std::unordered_map<std::wstring, std::shared_ptr<MaterialResource>> mMaterialTable;
    // Mesh
    std::vector<std::shared_ptr<MeshResource>> mMeshArray;
    std::unordered_map<std::wstring, std::shared_ptr<MeshResource>> mMeshTable;
    // Animation
    std::vector<std::shared_ptr<AnimationResource>> mAnimationArray;
    std::unordered_map<std::wstring, std::shared_ptr<AnimationResource>> mAnimationTable;
    // Bone
    std::vector<Bone*> mBoneArray;
    std::unordered_map<std::wstring, Bone*> mBoneTable;
    // Node
    std::vector<ModelNode*> mModelNodeArray;
    std::unordered_map<std::wstring, ModelNode*> mModelNodeTable;

    ModelNode* mRootNode;
};

class ModelNode
{
public:
    explicit ModelNode(std::wstring_view _name, ModelNode* _pParent);
    ~ModelNode() = default;
public:
    // 노드 이름 (본과 애니메이션 채널 등을 매칭해줌)
    std::wstring mNodeName;
    // 원본 매트릭스 값
    Matrix      mOriginMatrix = Matrix::Identity;
    Vector3     mOrginPosition;
    Quaternion  mOrginRotatin;
    Vector3     mOrginScale;
    // 부모 노드
    ModelNode* mParent;
    // 자식 노드
    std::vector<ModelNode*> mChildren;
    // 노드의 메쉬와 머티리얼(당연하게도 여러개가 쓰일 수 있다)
    std::vector<std::wstring> mMeshResources;
    std::vector<std::wstring> mMatResources;
    friend class FBXImporter;
};