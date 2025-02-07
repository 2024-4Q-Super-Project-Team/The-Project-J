#include "pch.h"
#include "FBXModel.h"
#include "Manager/GameManager.h"
#include "Resource/ResourceManager.h"
#include "Resource/FBXImporter/FBXImporter.h"

#include "Resource/Graphics/Material/Material.h"

FBXModelResource::FBXModelResource(ResourceHandle _handle)
    : Resource(_handle)
    , mRootNode(nullptr)
    , mModelPrefab(nullptr)
{
	SetEID("FBX Model : " + Helper::ToString(_handle.GetKey()));

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
   
	//for (auto& handle : pModel->TextureHandleTable)
	//{
	//	ResourceManager::RegisterResourceHandle(handle);
	//	ResourceManager::Alloc_Resource(handle);
	//}

	LoadJson();

	for (auto& resource : mMaterialArray)
	{
		resource->Create();
		ResourceManager::PushResource(resource);
	}
	for (auto& resource : mMeshArray)
	{
		resource->Create();
		ResourceManager::PushResource(resource);
	}
	for (auto& resource : mAnimationArray)
	{
		resource->Create();
		ResourceManager::PushResource(resource);
	}

	// Path 추가하기
	ResourceHandle handle = { eResourceType::PrefabResource, GetKey() + L"_Prefab", L"", _handle.GetPath()};
	mModelPrefab = new PrefabResource(handle, this);
	ResourceManager::PushResource(mModelPrefab);

	pModel->Clear();
}

FBXModelResource::~FBXModelResource()
{
	// FBX가 쓰는 리소스는 전부 해제한다
	for (auto& resource : mMaterialArray)
	{
		ResourceManager::Free_Resource(resource->GetHandle());
	}
	mMaterialArray.clear();
	for (auto& resource : mMeshArray)
	{
		ResourceManager::Free_Resource(resource->GetHandle());
	}
	mMeshArray.clear();
	for (auto& resource : mAnimationArray)
	{
		ResourceManager::Free_Resource(resource->GetHandle());
	}
	mAnimationArray.clear();
    SAFE_DELETE_ARRAY(mBoneArray);
    SAFE_DELETE_ARRAY(mModelNodeArray);
	ResourceManager::Free_Resource(mModelPrefab->GetHandle());
}

json FBXModelResource::Serialize()
{
	json ret;
	// Material
	ret["id"] = GiveId();
	for (auto& mat : mMaterialTable)
	{
		json matJson;
		matJson["name"]		= Helper::ToString(mat.first);
		matJson["material"] = mat.second->Serialize();
		ret["materials"] += matJson;
	}
	return ret;
}

void FBXModelResource::Deserialize(json& j)
{
	if(j.contains("id"))
		SetId(j["id"].get<unsigned int>());

	if (j.contains("materials"))
	{
		for (json& jsonData : j["materials"])
		{
			// Material Deserialize
			if (jsonData.contains("material") && jsonData.contains("name"))
			{
				std::wstring name = Helper::ToWString(jsonData["name"].get<std::string>());
				auto matResorce = Helper::FindMap(name, mMaterialTable);
				if (matResorce)
				{
					(*matResorce)->Deserialize(jsonData["material"]);
				}
			}
		}
	}

}

void FBXModelResource::SaveJson()
{
	std::filesystem::path JsonPath =
		std::filesystem::path(Helper::ToString(GetPath())).replace_extension(".json");

	if (!std::filesystem::exists(JsonPath) || !std::filesystem::is_regular_file(JsonPath)) {
		// JSON 파일이 없거나, 일반 파일이 아니면 새로운 JSON 파일을 생성
		std::ofstream createFile(JsonPath);
		//if (createFile.is_open()) {
		//	createFile << "{}"; // 기본 빈 JSON 데이터로 초기화
		//	createFile.close();
		//}
	}
	std::ofstream JsonStream(JsonPath);
	if (JsonStream.is_open())
	{
		json FBXJson = Serialize();
		JsonStream << FBXJson.dump(4);
		JsonStream.close();
	}
}

void FBXModelResource::LoadJson()
{
	std::filesystem::path JsonPath = 
		std::filesystem::path(Helper::ToString(GetPath())).replace_extension(".json");

	//if (!std::filesystem::exists(JsonPath) || !std::filesystem::is_regular_file(JsonPath)) {
		// JSON 파일이 없거나, 일반 파일이 아니면 새로운 JSON 파일을 생성
		//std::ofstream createFile(JsonPath);
		//if (createFile.is_open()) {
		//	createFile << "{}"; // 기본 빈 JSON 데이터로 초기화
		//	createFile.close();
		//}
	//}

	std::ifstream loadFile(JsonPath);
	json FBXJson;
	if (loadFile.is_open())
	{
		loadFile >> FBXJson;
		Deserialize(FBXJson);
		loadFile.close();
	}
}

void FBXModelResource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(GetKey());

	switch (_viewerType)
	{
	case EditorViewerType::DEFAULT:
	{
		std::string name = Helper::ToString(GetKey());
		ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
		auto flags = ImGuiSelectableFlags_AllowDoubleClick;
		if (ImGui::Selectable(GetEID(), false, flags))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				EditorManager::GetInspectorViewer()->SetFocusObject(this);
			}
		}
		EditorItemState state;
		state.mResourcePtr = this;
		state.mName = Helper::ToString(mHandle.GetKey());
		EditorDragNDrop::SendDragAndDropData(GetEID(), state);
		EDITOR_COLOR_POP(1);
		break;
	}
	case EditorViewerType::HIERARCHY:
		break;
	case EditorViewerType::INSPECTOR:
	{
		Resource::EditorRendering(_viewerType);

		if (mMaterialArray.empty() == false)
		{
			if (ImGui::TreeNodeEx("Material", EDITOR_FLAG_RESOURCE))
			{
				for (auto& Material : mMaterialArray)
				{
					if (Material)
						Material->EditorRendering(EditorViewerType::DEFAULT);
				}
				ImGui::TreePop();
			}
		}
		if (mMeshArray.empty() == false)
		{
			if (ImGui::TreeNodeEx("Mesh", EDITOR_FLAG_RESOURCE))
			{
				for (auto& Mesh : mMeshArray)
				{
					if (Mesh)
						Mesh->EditorRendering(EditorViewerType::DEFAULT);
				}
				ImGui::TreePop();
			}
		}
		if (mAnimationArray.empty() == false)
		{
			if (ImGui::TreeNodeEx("Animation", EDITOR_FLAG_RESOURCE))
			{
				for (auto& Animation : mAnimationArray)
				{
					if (Animation)
						Animation->EditorRendering(EditorViewerType::DEFAULT);
				}
				ImGui::TreePop();
			}
		}
		if (mModelPrefab)
		{
			if (ImGui::TreeNodeEx("Prefab", EDITOR_FLAG_RESOURCE))
			{
				mModelPrefab->EditorRendering(EditorViewerType::DEFAULT);
				ImGui::TreePop();
			}
		}
		ImGui::Text(("Num Node : " + std::to_string(mModelNodeArray.size())).c_str());
		break;
	}
	default:
		break;
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
