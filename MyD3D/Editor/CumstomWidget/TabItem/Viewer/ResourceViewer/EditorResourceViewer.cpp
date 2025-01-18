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