#include "pch.h"
#include "EditorResourceViewer.h"
#include "Resource/ResourceManager.h"
// Editor
#include "Editor/Handler/EditorDragNDrop.h"

Editor::ResourceViewer::ResourceViewer()
{
	for (size_t i = 0; i < (size_t)eResourceType::SIZE; ++i)
	{
		mResourceStringArray.push_back(ResourceTypeToStr((eResourceType)i));
	}
}

Editor::ResourceViewer::~ResourceViewer()
{
}

void Editor::ResourceViewer::Render()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name;
	//////////////////////////////////////////////////////////////////////////////////////
	// Resource Load List
	//////////////////////////////////////////////////////////////////////////////////////
	auto& loadList = ResourceManager::GetLoadResourceList();
	ImGui::Text("Load Resource List");
	for (ResourceHandle handle : loadList)
	{
		ImGui::Text(Helper::ToString(handle.GetKey() + L" : " + handle.GetPath()).c_str());
	}
	ImGui::Separator();
	//////////////////////////////////////////////////////////////////////////////////////
	// Resource Handle & Reosource View
	//////////////////////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < (size_t)eResourceType::SIZE; ++i)
	{
		std::string ResourceName = ResourceTypeToStr((eResourceType)i);
		if (ImGui::TreeNodeEx((ResourceName).c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Selected))
		{
			auto& table = ResourceManager::GetResourceTable((eResourceType)i);
			for (auto& [handle, resource] : table)
			{
				bool isSelected = InspectorViewer::IsFocusObject(resource);
				std::string widgetID = ResourceName + " : " + (Helper::ToString(handle.GetKey())).c_str();
				if (resource)
				{
					isSelected ?
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_RESOURCE_SELECTED) :
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_RESOURCE);
				}
				else
				{
					isSelected ?
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL_SELECTED) :
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
				}
				
				auto flags = ImGuiSelectableFlags_Highlight | ImGuiSelectableFlags_AllowDoubleClick;
				if (ImGui::Selectable((widgetID + uid).c_str(), isSelected, flags))
				{
					if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					{
						Editor::InspectorViewer::SetFocusObject(resource);
					}
				}
				EDITOR_COLOR_POP(1);
				////////////////////////////////////////////////////////////////////////////
				// Drag & Drop
				////////////////////////////////////////////////////////////////////////////
				EditorItemState state;
				state.mResourcePtr = resource;
				state.mName = Helper::ToString(handle.GetKey());
				EditorDragNDrop::SendDragAndDropData((widgetID + uid).c_str(), state);
			}
			ImGui::TreePop();
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	// Add Resource
	//////////////////////////////////////////////////////////////////////////////////////
	{
		//ImVec2 buttonSize = ImVec2(120, 30); // 버튼 크기 
		//if (ImGui::Button(("Add Resource" + uid).c_str(), buttonSize))
		//{
		//	ImGui::OpenPopup("Resource Register"); // 버튼 클릭 시 팝업 열기
		//}
		//if (ImGui::BeginPopup("Resource Register"))
		//{
		//	eResourceType TypeData;
		//	std::wstring MainKeyData;
		//	std::wstring PathData;
		//
		//	static int TypeIndex = 0; // 선택된 리소스 타입 (인덱스)
		//	// 리소스 타입 (콤보 박스)
		//	ImGui::Text("ResourceType : ");
		//	ImGui::Combo((uid + "RenderMode").c_str(), &TypeIndex, mResourceStringArray.data(), mResourceStringArray.size());
		//	TypeData = (eResourceType)TypeIndex;
		//
		//	static char MainKeybuffer[128] = "";
		//	ImGui::Text("MainKey : ");
		//	if (ImGui::InputText("##Add_Resource_MainKey", MainKeybuffer, sizeof(MainKeybuffer)))
		//	{
		//	}
		//	MainKeyData = Helper::ToWString(std::string(MainKeybuffer));
		//
		//	static char Pathbuffer[128] = "";
		//	ImGui::Text("Path : ");
		//	if (ImGui::InputText("##Add_Resource_Path", Pathbuffer, sizeof(Pathbuffer)))
		//	{
		//	}
		//	PathData = Helper::ToWString(std::string(Pathbuffer));
		//
		//	ImGui::NewLine();
		//	ImGui::Separator();
		//	buttonSize = ImVec2(70, 20); // 버튼 크기 
		//	const char* defaultSet = "";
		//	if (ImGui::Button(("OK" + uid).c_str(), buttonSize))
		//	{
		//		ResourceHandle handle = { TypeData, MainKeyData, L"", PathData };
		//		ResourceManager::RegisterResourceHandle(handle);
		//		strcpy_s(MainKeybuffer, defaultSet);
		//		strcpy_s(Pathbuffer, defaultSet);
		//	}
		//	ImGui::SameLine();
		//	if (ImGui::Button(("NO" + uid).c_str(), buttonSize))
		//	{
		//		strcpy_s(MainKeybuffer, defaultSet);
		//		strcpy_s(Pathbuffer, defaultSet);
		//		ImGui::CloseCurrentPopup();
		//	}
		//	// 팝업 내에서 마우스 오른쪽 클릭 시 팝업 닫기
		//	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
		//		ImGui::CloseCurrentPopup();
		//	}
		//	ImGui::EndPopup();
		//}
	}
}