#include "pch.h"
#include "FBXImporter.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"
#include "Resource/Graphics/Animation/Animation.h"
#include "Resource/Prefab/Prefab.h"

Assimp::Importer    FBXImporter::mImporter;
UINT			    FBXImporter::mFlag;
FBXResource* FBXImporter::mFBXResource;

BOOL FBXImporter::Initialize()
{
    mFBXResource = new FBXResource;
    mFlag =
        aiProcess_Triangulate |				// 삼각형 정점설정
        aiProcess_GenNormals |				// 노말벡터 생성
        aiProcess_GenUVCoords |				// 텍스쳐 좌표 생성
        aiProcess_CalcTangentSpace |		// 탄젠트 벡터 생성	
        aiProcess_LimitBoneWeights |	    // 영향을 받는 본 개수 최대 4개 제한
        aiProcess_ConvertToLeftHanded;		// 왼손 좌표계 변환
    return (BOOL)mImporter.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
}

void FBXImporter::Finalizaiton()
{
    SAFE_DELETE(mFBXResource);
}

FBXResource* FBXImporter::ImportFBXModel_All(const ResourceHandle& _handle)
{
    mFBXResource->Clear();
    std::string path = Helper::ToString(_handle.GetPath());
    const aiScene* pAiScene = mImporter.ReadFile(path, mFlag);
    if (pAiScene)
    {
        mFBXResource->Clear();
        mFBXResource->Handle = _handle;
        ProcessMaterial(pAiScene);
        ProcessMesh(pAiScene);
        ProcessAnimation(pAiScene);
        ProcessModelNode(pAiScene->mRootNode, mFBXResource->RootNode);
    }
    return mFBXResource;
}

std::wstring FBXImporter::AIToWString(const aiString& _orig)
{
    std::wstring wstr;
    wstr.assign(_orig.C_Str(), _orig.C_Str() + _orig.length);
    return wstr;
}

void FBXImporter::ProcessMaterial(const aiScene* _pAiScene)
{
    mFBXResource->MaterialArray.reserve(_pAiScene->mNumMaterials);
    mFBXResource->MaterialTable.reserve(_pAiScene->mNumMaterials);
    for (UINT mId = 0; mId < _pAiScene->mNumMaterials; ++mId)
    {
        aiMaterial* pAiMaterial = _pAiScene->mMaterials[mId];
        std::wstring Name = AIToWString(pAiMaterial->GetName());
        std::filesystem::path FBXPath = mFBXResource->Handle.GetPath();
        std::wstring MainKey = FBXPath.filename().wstring() + L"_" + Name;
        std::filesystem::path ResultPath = (FBXPath.parent_path() / MainKey).wstring() + L".json";
        FBXPath = FBXPath.parent_path();

        ResourceHandle handle =
        {
            eResourceType::MaterialResource,
            MainKey, Name, ResultPath,
            mFBXResource->Handle.GetKey()
        };
        MaterialResource* pMaterial = new MaterialResource(handle);

        aiString aiPath;
        // GetTexturePath
        {
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::DIFFUSE, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::SPECULAR, handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_AMBIENT, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::AMBIENT, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::EMISSIVE, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::NORMAL, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::ROUGHNESS, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_OPACITY, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::OPACITY, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_METALNESS, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::METALNESS, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
            if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &aiPath)) {
                std::filesystem::path newTag = aiPath.C_Str();
                std::filesystem::path newPath = FBXPath / newTag.filename();
                ResourceHandle handle = { eResourceType::Texture2DResource, newTag.filename().wstring(), L"", newPath.wstring() };
                pMaterial->SetMateirlaMapHandle(eMaterialMapType::AMBIENT_OCCLUSION, handle);
                mFBXResource->TextureHandleTable.push_back(handle);
            }
        }
        // GetProperty
        {
            //aiMaterialProperty* pAiProperty;
            MaterialProperty MatProp;
            aiColor4D   colorVal;
            float       floatVal;
            // Diffuse Color
            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, colorVal)) {
                MatProp.DiffuseRGB = ColorF(&colorVal.r);
            }

            // Ambient Color
            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, colorVal)) {
                MatProp.AmbientRGB = ColorF(&colorVal.r);
            }

            // Specular Color
            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, colorVal)) {
                MatProp.SpecularRGB = ColorF(&colorVal.r);
            }

            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, floatVal)) {
                MatProp.RoughnessScale = Clamp(floatVal, 0.0f, 1.0f);
            }

            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_METALLIC_FACTOR, floatVal)) {
                MatProp.MetallicScale = Clamp(floatVal, 0.0f, 1.0f);
            }
            pMaterial->SetMaterialProperty(&MatProp);
        }
        // Blending Mode
        {
            // Opacity Check
            float floatVal = 1.0f;
            if (AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_OPACITY, floatVal))
            {
                // Alpha값이 1.0 미만이면
                if (floatVal < 1.0f)
                {
                    // Transparent 처리
                    pMaterial->SetBlendingMode(eBlendModeType::TRANSPARENT_BLEND);
                }
                else
                {
                    // Opaque 처리
                    pMaterial->SetBlendingMode(eBlendModeType::OPAQUE_BLEND);
                }
            }
        }
        mFBXResource->MaterialArray.push_back(pMaterial);
        mFBXResource->MaterialTable[Name] = pMaterial;
    }
}

void FBXImporter::ProcessMesh(const aiScene* _pAiScene)
{
    mFBXResource->MeshArray.reserve(_pAiScene->mNumMeshes);
    mFBXResource->MeshTable.reserve(_pAiScene->mNumMeshes);
    for (UINT mIdx = 0; mIdx < _pAiScene->mNumMeshes; ++mIdx)
    {
        aiMesh* pAiMesh = _pAiScene->mMeshes[mIdx];
        std::vector<Vertex> vertexs;
        std::vector<UINT>   indices;
        UINT                MaterialIndex = pAiMesh->mMaterialIndex;
        std::wstring        Name = AIToWString(pAiMesh->mName) + L"_" + std::to_wstring(MaterialIndex);

        std::filesystem::path FBXPath = mFBXResource->Handle.GetPath();
        std::wstring MainKey = FBXPath.filename().wstring() + L"_" + Name;
        std::filesystem::path ResultPath = (FBXPath.parent_path() / MainKey).wstring() + L".json";

        vertexs.reserve(pAiMesh->mNumVertices);
        // Create Vertex
        for (UINT i = 0; i < pAiMesh->mNumVertices; i++)
        {
            Vertex vertex;
            if (pAiMesh->HasPositions())
            {
                memcpy(&vertex.Position, &pAiMesh->mVertices[i], sizeof(aiVector3D));
            }
            if (pAiMesh->HasNormals())
            {
                memcpy(&vertex.Normal, &pAiMesh->mNormals[i], sizeof(aiVector3D));
            }
            if (pAiMesh->HasTangentsAndBitangents())
            {
                memcpy(&vertex.Tangent, &pAiMesh->mTangents[i], sizeof(aiVector3D));
                memcpy(&vertex.BiTangent, &pAiMesh->mBitangents[i], sizeof(aiVector3D));
            }
            if (pAiMesh->HasTextureCoords(0))
            {
                memcpy(&vertex.TexCoord, &pAiMesh->mTextureCoords[0][i], sizeof(aiVector2D));
            }
            vertexs.push_back(vertex);
        }
        // Index
        for (UINT i = 0; i < pAiMesh->mNumFaces; i++) {
            aiFace& face = pAiMesh->mFaces[i];
            UINT indexCount = face.mNumIndices;
            for (UINT j = 0; j < indexCount; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        ResourceHandle handle =
        {
            eResourceType::MeshResource,
            MainKey, Name, ResultPath,
            mFBXResource->Handle.GetKey()
        };
        MeshResource* pMesh = new MeshResource(handle, vertexs, indices);

        mFBXResource->MeshArray.push_back(pMesh);
        mFBXResource->MeshTable[Name] = pMesh;
        mFBXResource->MaterialIndexTable.push_back(MaterialIndex);
        // 메쉬에 본 데이터 저장
        ProcessBone(pAiMesh);
    }
}

void FBXImporter::ProcessBone(const aiMesh* _pAiMesh)
{
    mFBXResource->BoneArray.reserve(_pAiMesh->mNumBones);
    mFBXResource->BoneTable.reserve(_pAiMesh->mNumBones);
    MeshResource* pMesh = mFBXResource->MeshArray.back();
    for (UINT bIdx = 0; bIdx < _pAiMesh->mNumBones; ++bIdx)
    {
        Bone* pBone;
        aiBone* pAibone = _pAiMesh->mBones[bIdx];
        std::wstring Name = AIToWString(pAibone->mName);

        std::filesystem::path FBXPath = mFBXResource->Handle.GetPath();
        std::wstring MainKey = FBXPath.filename().wstring() + L"_" + Name;

        auto itr = mFBXResource->BoneTable.find(Name);
        if (FIND_SUCCESS(itr, mFBXResource->BoneTable))
        {   // 모델리소스에서 이미 똑같은 이름의 본이 있으면 그것을 참조
            pBone = itr->second;
        }
        else
        {   // 모델리소스에서 해당 이름의 본이 없으면 만들고 그것을 참조
            Matrix boneMatrix = XMMatrixTranspose(Matrix(&pAibone->mOffsetMatrix.a1));
            pBone = new Bone(Name, boneMatrix);
            mFBXResource->BoneArray.push_back(pBone);
            mFBXResource->BoneTable[Name] = pBone;
        }
        // 메쉬에 해당 본을 넣어준다.
        pMesh->mBoneArray.push_back(pBone);
        pMesh->mBoneTable[Name] = pBone;
        // 현재 본이 영향을 주는 정점들에 본 인덱싱을 매핑해준다.
        for (UINT i = 0; i < pAibone->mNumWeights; i++)
        {
            UINT vertexID = pAibone->mWeights[i].mVertexId;
            float vertexWeight = pAibone->mWeights[i].mWeight;
            pMesh->mVertices[vertexID].AddBoneData(bIdx, vertexWeight);
        }
    }
}

void FBXImporter::ProcessAnimation(const aiScene* _pAiScene)
{
    for (UINT animIndex = 0; animIndex < _pAiScene->mNumAnimations; ++animIndex)
    {
        aiAnimation* pAiAnim = _pAiScene->mAnimations[animIndex];
        std::wstring Name = AIToWString(pAiAnim->mName);

        std::filesystem::path FBXPath = mFBXResource->Handle.GetPath();
        std::wstring MainKey = FBXPath.filename().wstring() + L"_" + Name;
        std::filesystem::path ResultPath = (FBXPath.parent_path() / MainKey).wstring() + L".json";

        ResourceHandle handle = 
        {
            eResourceType::AnimationResource,
            MainKey, Name, ResultPath,
            mFBXResource->Handle.GetKey()
        };
        AnimationResource* pAnim = new AnimationResource(handle);
        pAnim->SetFramePerSecond((float)pAiAnim->mTicksPerSecond);
        pAnim->SetTotalFrame((float)pAiAnim->mDuration);
        mFBXResource->AnimationArray.push_back(pAnim);
        mFBXResource->AnimationTable[Name] = pAnim;
        ProcessFrame(pAiAnim);
    }
}

void FBXImporter::ProcessFrame(const aiAnimation* _pAiAnim)
{
    AnimationResource* pAnim = mFBXResource->AnimationArray.back();
    for (UINT channelIndex = 0; channelIndex < _pAiAnim->mNumChannels; channelIndex++)
    {
        aiNodeAnim* animNode = _pAiAnim->mChannels[channelIndex];
        std::wstring Name = AIToWString(animNode->mNodeName);
        AnimationNode* pChannel = new AnimationNode(Name, pAnim);
        ProcessKey(animNode, pChannel);
        pAnim->AddChannel(pChannel);
    }
}

void FBXImporter::ProcessKey(const aiNodeAnim* _pAiNodeAnim, AnimationNode* _pChannel)
{
    for (UINT kIdx = 0; kIdx < _pAiNodeAnim->mNumPositionKeys; kIdx++)
    {
        Vector3		Position(&_pAiNodeAnim->mPositionKeys[kIdx].mValue.x);
        Vector3		Scale(&_pAiNodeAnim->mScalingKeys[kIdx].mValue.x);
        Quaternion	Rotation(
            _pAiNodeAnim->mRotationKeys[kIdx].mValue.x,
            _pAiNodeAnim->mRotationKeys[kIdx].mValue.y,
            _pAiNodeAnim->mRotationKeys[kIdx].mValue.z,
            _pAiNodeAnim->mRotationKeys[kIdx].mValue.w);
        _pChannel->mPositionKeys.emplace_back(Position, _pAiNodeAnim->mPositionKeys[kIdx].mTime);
        _pChannel->mScalingKeys.emplace_back(Scale, _pAiNodeAnim->mScalingKeys[kIdx].mTime);
        _pChannel->mRotationKeys.emplace_back(Rotation, _pAiNodeAnim->mRotationKeys[kIdx].mTime);
    }
}

void FBXImporter::ProcessModelNode(const aiNode* _pAiNode, ModelNode* _pNode)
{
    std::wstring Name = AIToWString(_pAiNode->mName);
    ModelNode* pNode = new ModelNode(Name, _pNode);
    if (!_pNode)
    {
        mFBXResource->RootNode = pNode;
    }
    // 해당 노드의 원래 매트릭스(바인드 포즈)
    pNode->mOriginMatrix = XMMatrixTranspose(Matrix(&_pAiNode->mTransformation.a1));
    // 포지션, 회전, 스케일 벡터로 변환
    Helper::DecomposeMatrix(pNode->mOriginMatrix,
        pNode->mOrginPosition,
        pNode->mOrginRotatin,
        pNode->mOrginScale);
    for (UINT i = 0; i < _pAiNode->mNumMeshes; ++i)
    {
        pNode->mMeshResources.push_back(mFBXResource->MeshArray[_pAiNode->mMeshes[i]]->GetSubKey());
        pNode->mMatResources.push_back(mFBXResource->MaterialArray[mFBXResource->MaterialIndexTable[_pAiNode->mMeshes[i]]]->GetSubKey());
    }
    mFBXResource->ModelNodeArray.push_back(pNode);
    mFBXResource->ModelNodeTable[Name] = pNode;
    for (UINT i = 0; i < _pAiNode->mNumChildren; ++i)
    {
        ProcessModelNode(_pAiNode->mChildren[i], pNode);
    }
}
