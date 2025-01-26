#pragma once
#include "Resource/Resource.h"

class MeshResource;
class MaterialResource;
class AnimationResource;
class Bone;
class ModelNode;
class PrefabResource;

// �� ��� ��â����. ���� ��� ������ ������ �ִ� ���� ��. ����
// ����Ƽ�� �ƹ�Ÿ ���� �����̶� �����ϸ� �ɵ�
class FBXModelResource 
    : public Resource
{
public:
    RESOURCE_TYPE(FBXModelResource);
    explicit FBXModelResource(ResourceHandle _handle);
    virtual ~FBXModelResource();
public:
    bool isEmpty() { return !(mRootNode == nullptr); }
public:
    // Material
    std::vector<MaterialResource*> mMaterialArray;
    std::unordered_map<std::wstring, MaterialResource*> mMaterialTable;
    // Mesh
    std::vector<MeshResource*> mMeshArray;
    std::unordered_map<std::wstring, MeshResource*> mMeshTable;
    // Animation
    std::vector<AnimationResource*> mAnimationArray;
    std::unordered_map<std::wstring, AnimationResource*> mAnimationTable;
    // Bone
    std::vector<Bone*> mBoneArray;
    std::unordered_map<std::wstring, Bone*> mBoneTable;
    // Node
    std::vector<ModelNode*> mModelNodeArray;
    std::unordered_map<std::wstring, ModelNode*> mModelNodeTable;

    ModelNode* mRootNode;

    PrefabResource* mModelPrefab;
public:
    virtual json Serialize();
    virtual void Deserialize(json& j);
    void SaveJson();
    void LoadJson();
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
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