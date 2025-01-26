#include "pch.h"
#include "EditorJsonWriter.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"
#include "Save/SaveManager.h"
#include "ViewportScene/ViewportScene.h"

namespace Editor
{
	JsonWriter::JsonWriter()
	{
		mlabel = ("Save File##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
	}

	JsonWriter::~JsonWriter()
	{
	}

	void JsonWriter::Render()
	{
		std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
		if (ImGui::MenuItem(mlabel.c_str()))
		{
			int result = MessageBox(
				EditorManager::mEditorViewport->GetIWindow()->GetHandle(), // �θ� â �ڵ� (NULL�� �ϸ� ������ �޽��� �ڽ�)
				L"���� ���� �����Ͻðڽ��ϱ�?",				// �޽��� �ؽ�Ʈ
				L"���� ���� ������ ���� �������µ�...",		// �޽��� �ڽ� ����
				MB_YESNO | MB_ICONWARNING					// ��Ÿ��: ��/�ƴϿ� ��ư + ���� ������
			);

			if (result == IDYES)
			{
				SaveManager::Save();
			}

		}
	}
}
