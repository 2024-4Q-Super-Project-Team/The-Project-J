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
				EditorManager::mEditorViewport->GetIWindow()->GetHandle(), // 부모 창 핸들 (NULL로 하면 독립적 메시지 박스)
				L"정말 정말 저장하시겠습니까?",				// 메시지 텍스트
				L"이전 저장 내용은 덮어 씌어지는데...",		// 메시지 박스 제목
				MB_YESNO | MB_ICONWARNING					// 스타일: 예/아니오 버튼 + 질문 아이콘
			);

			if (result == IDYES)
			{
				SaveManager::Save();
			}

		}
	}
}
