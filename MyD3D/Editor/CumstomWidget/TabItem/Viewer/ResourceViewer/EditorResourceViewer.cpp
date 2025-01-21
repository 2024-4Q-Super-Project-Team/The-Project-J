#include "pch.h"
#include "EditorResourceViewer.h"
#include "Resource/ResourceManager.h"
// Editor
#include "Editor/Handler/EditorDragNDrop.h"

Editor::ResourceViewer::ResourceViewer()
{
}

Editor::ResourceViewer::~ResourceViewer()
{
}

void Editor::ResourceViewer::Render()
{
	for (size_t i = 0; i < (size_t)eResourceType::SIZE; ++i)
	{
		std::string ResourceName = ResourceTypeToStr((eResourceType)i);
		if (ImGui::TreeNodeEx((ResourceName).c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Selected))
		{
			static char buffer[128] = "";
			const char* defaultPath = "resource/";
			strcpy_s(buffer, defaultPath);

			ImGui::Text(("Add Resource " + ResourceName).c_str());
			if (ImGui::InputText(("##Add_" + ResourceName).c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				const std::wstring resourcePath = Helper::ToWString(std::string(buffer));
			}

			ImGui::Separator();

			ResourceTable& table = ResourceManager::GetResourceTable((eResourceType)i);
			for (auto& resource : table)
			{
				if (resource.second.expired())
					continue;
				resource.second.lock()->EditorRendering();
			}
			ImGui::TreePop();
		}
	}
}