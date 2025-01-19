#pragma once

class FBXModelResource;
class MeshResource;
class MaterialResource;
class AnimationResource;
class AnimationNode;
class Bone;
class ModelNode;


struct FBXResource;

class FBXImporter
{
public:
    static Assimp::Importer mImporter;
    static UINT			    mFlag;
public:
    static BOOL Initialize();
    static void Finalizaiton();
public:
    static inline FBXResource* GetFBDXResource() { return mFBXResource; }
public:
    static FBXResource* ImportFBXModel_All(const ResourceHandle& _handle);
    static FBXResource* ImportFBXModel_Mesh(const std::wstring _path);
    static FBXResource* ImportFBXModel_Material(const std::wstring _path);
    static FBXResource* ImportFBXModel_Animation(const std::wstring _path);
private:
    static void ProcessMaterial(const aiScene* _pAiScene);
    static void ProcessMesh(const aiScene* _pAiScene);
    static void ProcessBone(const aiMesh* _pAiMesh);
    static void ProcessAnimation(const aiScene* _pAiScene);
    static void /**/ProcessFrame(const aiAnimation* _pAiAnim);
    static void /*  */ProcessKey(const aiNodeAnim* _pAiNodeAnim, AnimationNode* _pChannel);
    static void ProcessModelNode(const aiNode* _pAiNode, ModelNode* _pNode);
private:
    // aistring을 wstring으로 변환해주는 함수.
    static std::wstring AIToWString(const aiString& _orig);
private:
    static FBXResource* mFBXResource;
};

struct FBXResource
{
    void Clear() {
        Handle = {};
        MaterialArray.clear();  MaterialTable.clear();
        MeshArray.clear();      MeshTable.clear();
        BoneArray.clear();      BoneTable.clear();
        AnimationArray.clear(); AnimationTable.clear();
        ModelNodeArray.clear(); ModelNodeTable.clear();
        MaterialIndexTable.clear();
        RootNode = nullptr;
    }
    ResourceHandle Handle;
    // Material
    std::vector<std::shared_ptr<MaterialResource>> MaterialArray;
    std::unordered_map<std::wstring, std::shared_ptr<MaterialResource>> MaterialTable;
    std::vector<UINT> MaterialIndexTable;
    // Mesh
    std::vector<std::shared_ptr<MeshResource>> MeshArray;
    std::unordered_map<std::wstring, std::shared_ptr<MeshResource>> MeshTable;
    // Bone
    std::vector<Bone*> BoneArray;
    std::unordered_map<std::wstring, Bone*> BoneTable;
    // Animation
    std::vector<std::shared_ptr<AnimationResource>> AnimationArray;
    std::unordered_map<std::wstring, std::shared_ptr<AnimationResource>> AnimationTable;
    // Node
    ModelNode* RootNode;
    std::vector<ModelNode*> ModelNodeArray;
    std::unordered_map<std::wstring, ModelNode*> ModelNodeTable;
};