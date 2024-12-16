#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class IGraphicsResource;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class Texture;
	class MeshInfo;
	class MaterialInfo;
	class ModelInfo;
	class ModelNodeInfo;
	class AnimationInfo;
	class ChannelInfo;

	struct Vertex;
}

class GameManager;

class ResourceManager
{
public:
	ResourceManager(GameManager* _pGameMng);
	~ResourceManager();
public:
	std::shared_ptr<Graphics::Texture> GetTextureData(std::wstring_view _path);
	std::shared_ptr<Graphics::ModelInfo> GetModelData(std::wstring_view _path);
private:
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::InputLayout>>		 mInputLayout;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::VertexShader>>		 mVertexShader;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::PixelShader>>		 mPixelShader;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::Texture>>			 mTextures;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::ModelInfo>>		 mModels;

	Assimp::Importer	 mAssimpImporter;
	UINT				 mImpoterFlag;
	GameManager* const	 mGameManager;
private:
	void AIGetModelData(std::wstring_view _path, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	
	void AIGetMaterialData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	
	void AIGetMeshData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	
	void AIGetBoneData(const aiMesh* _pAiMesh, std::vector<Graphics::Vertex> _vertex, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	
	void AIGetAnimationData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	void AIGetAnimFrameData(const aiAnimation* _pAiAnim, std::weak_ptr<Graphics::AnimationInfo> _wpAnimInfo);
	void AIGetKeyFrameData(const aiNodeAnim* _pAiNodeAnim, Graphics::ChannelInfo& _channelInfo);
	
	void AIGetNodeData(const aiNode* _pAiNode, Graphics::ModelNodeInfo* _pParentNode, std::weak_ptr<Graphics::ModelInfo> _wpModel);
	
	inline void AIToWString(const aiString& _orig, std::wstring& _dest);
public:
};

