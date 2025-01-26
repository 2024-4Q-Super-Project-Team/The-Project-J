#include "pch.h"
#include "EditorJsonLoader.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"
#include "Save/SaveManager.h"
#include "ViewportScene/ViewportScene.h"

Editor::JsonLoader::JsonLoader()
{
	mlabel = ("Load File##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::JsonLoader::~JsonLoader()
{
}

void Editor::JsonLoader::Render()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	if (ImGui::MenuItem(mlabel.c_str()))
	{
		int result = MessageBox(
			EditorManager::mEditorViewport->GetIWindow()->GetHandle(), // �θ� â �ڵ� (NULL�� �ϸ� ������ �޽��� �ڽ�)
			L"���� ���� �ҷ����ðڽ��ϱ�?",				// �޽��� �ؽ�Ʈ
			L"�������� ���� ������ ���󰩴ϴ�........", // �޽��� �ڽ� ����
			MB_YESNO | MB_ICONWARNING					// ��Ÿ��: ��/�ƴϿ� ��ư + ���� ������
		);

		if (result == IDYES)
		{
			SaveManager::Load();
		}

	}
}
