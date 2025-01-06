#pragma once
#include "Resource/Resource.h"

class MeshResource;
class MaterialResource;
class AnimationResource;
class Bone;
class ModelNode;
class Prefab;

// �� ��� ��â����. ���� ��� ������ ������ �ִ� ���� ��. ����
// ����Ƽ�� �ƹ�Ÿ ���� �����̶� �����ϸ� �ɵ�
class FBXModelResource : public Resource
{
public:
    RESOURCE_TYPE(eResourceType::FBXModel);
    explicit FBXModelResource(std::wstring_view _name);
    virtual ~FBXModelResource();
public:
    // �̰� ���� �� shared_ptr��? ���ʿ䰡 ������ ����
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
    // ��� �̸� (���� �ִϸ��̼� ä�� ���� ��Ī����)
    std::wstring mNodeName;
    // ���� ��Ʈ���� ��
    Matrix      mOriginMatrix = Matrix::Identity;
    Vector3     mOrginPosition;
    Quaternion  mOrginRotatin;
    Vector3     mOrginScale;
    // �θ� ���
    ModelNode* mParent;
    // �ڽ� ���
    std::vector<ModelNode*> mChildren;
    // ����� �޽��� ��Ƽ����(�翬�ϰԵ� �������� ���� �� �ִ�)
    std::vector<std::wstring> mMeshResources;
    std::vector<std::wstring> mMatResources;
    friend class FBXImporter;
};