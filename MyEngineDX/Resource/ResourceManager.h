#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class IGraphicsResource;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class Texture;

	class ModelResource;
	class ModelNodeResource;
	class MeshResource;
	class MaterialResource;
	class AnimationResource;
	class ChannelResource;

	struct Vertex;
}

class GameManager;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
public:
	std::shared_ptr<Graphics::Texture> GetTextureData(std::wstring_view _path);
	std::shared_ptr<Graphics::ModelResource> GetModelData(std::wstring_view _path);
private:
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::InputLayout>>		 mInputLayout;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::VertexShader>>		 mVertexShader;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::PixelShader>>		 mPixelShader;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::Texture>>			 mTextures;
	std::unordered_map<std::wstring, std::weak_ptr<Graphics::ModelResource>>	 mModels;

	Assimp::Importer	 mAssimpImporter;
	UINT				 mImpoterFlag;
private: // Assimp
	void AIGetModelData(std::wstring_view _path, std::weak_ptr<Graphics::ModelResource> _wpModel);
	
	void AIGetMaterialData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelResource> _wpModel);
	
	void AIGetMeshData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelResource> _wpModel);
	
	void AIGetBoneData(const aiMesh* _pAiMesh, std::vector<Graphics::Vertex> _vertex, std::weak_ptr<Graphics::ModelResource> _wpModel);
	
	void AIGetAnimationData(const aiScene* _pAiScene, std::weak_ptr<Graphics::ModelResource> _wpModel);
	void AIGetAnimFrameData(const aiAnimation* _pAiAnim, std::weak_ptr<Graphics::AnimationResource> _wpAnimResource);
	void AIGetKeyFrameData(const aiNodeAnim* _pAiNodeAnim, Graphics::ChannelResource& _channelResource);
	
	void AIGetNodeData(const aiNode* _pAiNode, Graphics::ModelNodeResource* _pParentNode, std::weak_ptr<Graphics::ModelResource> _wpModel);
	
	void AIToWString(const aiString& _orig, std::wstring& _dest);
};

