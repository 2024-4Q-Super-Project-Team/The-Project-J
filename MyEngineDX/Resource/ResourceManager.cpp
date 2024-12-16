#include "pch.h"
#include "Resource/ResourceManager.h"
#include "GameManager/GameManager.h"

ResourceManager::ResourceManager(GameManager* _pGameMng)
    :mGameManager(_pGameMng)
{
	mAssimpImporter.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
	mImpoterFlag =
		aiProcess_ConvertToLeftHanded |		// 왼손 좌표계 변환
		aiProcess_Triangulate |				// 삼각형 정점설정
		aiProcess_GenNormals |				// 노말벡터 생성
		aiProcess_GenUVCoords |				// 텍스쳐 좌표 생성
		aiProcess_CalcTangentSpace |		// 탄젠트 벡터 생성	
		aiProcess_LimitBoneWeights;			// 영향을 받는 본 개수 최대 4개 제한
}

ResourceManager::~ResourceManager()
{
}

std::shared_ptr<Graphics::Texture> ResourceManager::GetTextureData(std::wstring_view _path)
{
	std::shared_ptr<Graphics::Texture> pResource;
	auto find = mTextures.find(_path.data());
	if (find != mTextures.end())
	{
		pResource = find->second.lock();
		if (pResource)
		{
			return pResource;
		}
		mTextures.erase(find);
	}
	{
		// 지우고 새로 만들어서 리턴한다.
		Graphics::GraphicsDevice* pDevice = mGameManager->GetGraphicsManager()->GetDevice();
		pResource = std::make_shared<Graphics::Texture>(pDevice, _path.data());
		mTextures[_path.data()] = pResource;
	}
	return pResource;
}

std::shared_ptr<Graphics::ModelInfo> ResourceManager::GetModelData(std::wstring_view _path)
{
	std::shared_ptr<Graphics::ModelInfo> pResource;
	auto find = mModels.find(_path.data());
	if (find != mModels.end())
	{
		pResource = find->second.lock();
		if (pResource)
		{
			return pResource;
		}
		mModels.erase(find);
	}
	{
		// 지우고 새로 만들어서 리턴한다.
		pResource = std::make_shared<Graphics::ModelInfo>(_path);
		AIGetModelData(_path, pResource);
		mModels[_path.data()] = pResource;
	}

	return pResource;
}

void ResourceManager::AIGetModelData(std::wstring_view _path, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	std::string path;
	path.assign(_path.begin(), _path.end());
	const aiScene* pAiScene = mAssimpImporter.ReadFile(path, mImpoterFlag);
	if (pAiScene)
	{
		AIGetMaterialData(pAiScene, _wpModel);
		AIGetMeshData(pAiScene, _wpModel);
		AIGetAnimationData(pAiScene, _wpModel);
		AIGetNodeData(pAiScene->mRootNode, nullptr, _wpModel);
	}
}

void ResourceManager::AIGetMeshData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	Graphics::ModelInfo* pModel = _wpModel.lock().get();
	pModel->GetMeshs().reserve(_pAiScene->mNumMeshes);
	Graphics::GraphicsDevice* pDevice = mGameManager->GetGraphicsManager()->GetDevice();
	for (UINT meshIndex = 0; meshIndex < _pAiScene->mNumMeshes; ++meshIndex)
	{
		aiMesh* pAiMesh = _pAiScene->mMeshes[meshIndex];
		std::vector<Graphics::Vertex> vertexs;
		std::vector<UINT> indices;
		std::wstring meshName;
		AIToWString(pAiMesh->mName, meshName);
		vertexs.reserve(pAiMesh->mNumVertices);
		// Vertex
		for (UINT i = 0; i < pAiMesh->mNumVertices; i++)
		{
			Graphics::Vertex vertex;
			memcpy(&vertex.Position, &pAiMesh->mVertices[i], sizeof(aiVector3D));
			memcpy(&vertex.Normal, &pAiMesh->mNormals[i], sizeof(aiVector3D));
			memcpy(&vertex.Tangent, &pAiMesh->mTangents[i], sizeof(aiVector3D));
			memcpy(&vertex.TexCoord, &pAiMesh->mTextureCoords[0][i], sizeof(aiVector2D));
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
		AIGetBoneData(pAiMesh, vertexs, _wpModel);
		std::shared_ptr<Graphics::MeshInfo> pMesh 
			= std::make_shared<Graphics::MeshInfo>(pDevice, meshName, vertexs, indices);
		pModel->GetMeshs().push_back(std::make_pair(pAiMesh->mMaterialIndex, std::move(pMesh)));
	}
}

void ResourceManager::AIGetMaterialData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	Graphics::ModelInfo* pModel = _wpModel.lock().get();
	pModel->GetMaterials().reserve(_pAiScene->mNumMaterials);
	for (UINT materialIndex = 0; materialIndex < _pAiScene->mNumMaterials; ++materialIndex)
	{
		aiMaterial* pAiMaterial = _pAiScene->mMaterials[materialIndex];
		std::wstring materialName;
		AIToWString(pAiMaterial->GetName(), materialName);
		std::shared_ptr<Graphics::MaterialInfo> pMaterial
			= std::make_shared<Graphics::MaterialInfo>(materialName);
		Graphics::MaterialInfo* pMat = pMaterial.get();
		aiString		aiPath;
		std::wstring	path;
		/*if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath)) {
			AIToWString(aiPath, path);
			pMat->SetMaterialMap(Graphics::eMaterialMapType::DIFFUSE,
				GetTextureData(L"Resource/texture/" + path));
		}*/
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::SPECULAR,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_AMBIENT, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::AMBIENT,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::EMISSIVE,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::NORMAL,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::SHININESS,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_OPACITY, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::OPACITY,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		//if (AI_SUCCESS == pAiMaterial->GetTexture(aiTextureType_METALNESS, 0, &aiPath)) {
		//	AIToWString(aiPath, path);
		//	pMaterial->SetMaterialMap(Graphics::eMaterialMapType::METALNESS,
		//		GetTextureData(L"Resource/texture/" + path));
		//}
		pModel->GetMaterials().push_back(pMaterial);
	}
}

void ResourceManager::AIGetBoneData(const aiMesh* _pAiMesh, std::vector<Graphics::Vertex> _vertex, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	Graphics::ModelInfo* pModel = _wpModel.lock().get();
	for (UINT boneIndex = 0; boneIndex < _pAiMesh->mNumBones; ++boneIndex)
	{
		aiBone* pAibone = _pAiMesh->mBones[boneIndex];
		std::wstring boneName;
		AIToWString(pAibone->mName, boneName);
		std::shared_ptr<Graphics::BoneInfo> pBone;
		auto itr = pModel->GetBones().find(boneName);
		if (itr != pModel->GetBones().end())
		{
			pBone = itr->second;
		}
		else
		{
			UINT bondID = pModel->GetBones().size();
			Matrix boneMatrix = XMMatrixTranspose(Matrix(&pAibone->mOffsetMatrix.a1));
			pBone = std::make_shared<Graphics::BoneInfo>(boneName, bondID, boneMatrix);
			pModel->GetBones()[boneName] = std::move(pBone);
		}
		for (int i = 0; i < pAibone->mNumWeights; i++)
		{
			Graphics::Vertex& vertex = _vertex[pAibone->mWeights[i].mVertexId];
			for (int j = 0; j < 4; ++j)
			{
				if (vertex.BoneID[j] == -1)
				{
					vertex.BoneID[j] = pModel->GetBones()[boneName].get()->GetPaletteID();
					vertex.BoneWeight[j] = pAibone->mWeights[i].mWeight;
				}
			}
		}
	}
}

void ResourceManager::AIGetAnimationData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	Graphics::ModelInfo* pModel = _wpModel.lock().get();
	for (UINT animIndex = 0; animIndex < _pAiScene->mNumAnimations; ++animIndex)
	{
		std::shared_ptr<Graphics::AnimationInfo> pAnimInfo;
		aiAnimation* pAiAnim = _pAiScene->mAnimations[animIndex];
		std::wstring AnimName;
		AIToWString(pAiAnim->mName, AnimName);
		pAnimInfo = std::make_shared<Graphics::AnimationInfo>(AnimName);
		AIGetAnimFrameData(pAiAnim, pAnimInfo);
		pModel->GetAnimations()[AnimName] = pAnimInfo;
	}
}

void ResourceManager::AIGetAnimFrameData(const aiAnimation* _pAiAnim, std::weak_ptr<Graphics::AnimationInfo> _wpAnimInfo)
{
	for (UINT channelIndex = 0; channelIndex < _pAiAnim->mNumChannels; channelIndex++)
	{
		aiNodeAnim* animNode = _pAiAnim->mChannels[channelIndex];
		std::wstring ChannelName;
		AIToWString(_pAiAnim->mName, ChannelName);
		Graphics::ChannelInfo channelInfo(_wpAnimInfo, ChannelName);
		AIGetKeyFrameData(animNode, channelInfo);
	}
}

void ResourceManager::AIGetKeyFrameData(const aiNodeAnim* _pAiNodeAnim, Graphics::ChannelInfo& _channelInfo)
{
	for (UINT keyIndex = 0; keyIndex < _pAiNodeAnim->mNumPositionKeys; keyIndex++)
	{
		Vector3		Position(&_pAiNodeAnim->mPositionKeys[keyIndex].mValue.x);
		Vector3		Scale(&_pAiNodeAnim->mScalingKeys[keyIndex].mValue.x);
		Quaternion	Rotation(
			_pAiNodeAnim->mRotationKeys[keyIndex].mValue.x,
			_pAiNodeAnim->mRotationKeys[keyIndex].mValue.y,
			_pAiNodeAnim->mRotationKeys[keyIndex].mValue.z,
			_pAiNodeAnim->mRotationKeys[keyIndex].mValue.w);
		_channelInfo.mPositionKeys.emplace_back(Position, _pAiNodeAnim->mPositionKeys[keyIndex].mTime);
		_channelInfo.mScalingKeys.emplace_back(Scale, _pAiNodeAnim->mScalingKeys[keyIndex].mTime);
		_channelInfo.mRotationKeys.emplace_back(Rotation, _pAiNodeAnim->mRotationKeys[keyIndex].mTime);
	}
}

void ResourceManager::AIGetNodeData(const aiNode* _pAiNode, Graphics::ModelNodeInfo* _pParentNode, std::weak_ptr<Graphics::ModelInfo> _wpModel)
{
	Graphics::ModelInfo* pModel = _wpModel.lock().get();
	std::wstring nodeName;
	AIToWString(_pAiNode->mName, nodeName);
	Graphics::ModelNodeInfo* pNode = new Graphics::ModelNodeInfo(nodeName, _pParentNode);
	if (!pModel->mRootNode)
	{
		pModel->mRootNode = pNode;
	}
	for (int i = 0; i < _pAiNode->mNumMeshes; ++i)
	{
		pNode->mMeshInfos.push_back(pModel->GetMeshs()[_pAiNode->mMeshes[i]].second.get());
		int MappingIndex = pModel->GetMeshs()[_pAiNode->mMeshes[i]].first;
		pNode->mMaterials.push_back(pModel->GetMaterials()[MappingIndex].get());
	}
	pNode->mTransformation = XMMatrixTranspose(Matrix(&_pAiNode->mTransformation.a1));
	pModel->GetNodes()[nodeName] = pNode;
	for (int i = 0; i < _pAiNode->mNumChildren; ++i)
	{
		AIGetNodeData(_pAiNode->mChildren[i], pNode, _wpModel);
	}
}

inline void ResourceManager::AIToWString(const aiString& _orig, std::wstring& _dest)
{
	_dest.assign(_orig.C_Str(), _orig.C_Str() + _orig.length);
}