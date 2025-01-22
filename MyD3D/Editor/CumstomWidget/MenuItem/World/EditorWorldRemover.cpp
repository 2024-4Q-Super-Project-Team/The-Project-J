#include "pch.h"
#include "EditorWorldRemover.h"
#include "Editor/EditorManager.h"
#include "World/WorldManager.h"
#include "ViewportScene/ViewportScene.h"

Editor::WorldRemover::WorldRemover()
{
	mlabel = ("Delete World##" + std::to_string(reinterpret_cast<uintptr_t>(this))).c_str();
}

Editor::WorldRemover::~WorldRemover()
{
}

void Editor::WorldRemover::Render()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::BeginMenu(mlabel.c_str()))
    {
        auto pFocusWorldManager = EditorManager::GetHierarchyViewer()->GetFocusWorldManager();
        auto pWorldContainer = pFocusWorldManager->GetWorlds();
        for (auto& [name, world] : pWorldContainer)
        {
            std::string worldName = Helper::ToString(name);
            if (ImGui::MenuItem((worldName + uid).c_str()))
            {
                // 메시지 박스 표시
                int result = MessageBox(
                   EditorManager::mEditorViewport->GetIWindow()->GetHandle(), // 부모 창 핸들 (NULL로 하면 독립적 메시지 박스)
                    L"정말 정말 월드를 삭제하시겠습니까?",      // 메시지 텍스트
                    L"정말 다시 한번 고려해보십시오.",          // 메시지 박스 제목
                    MB_YESNO | MB_ICONWARNING                  // 스타일: 예/아니오 버튼 + 질문 아이콘
                );

                // 사용자의 선택 결과 처리
                if (result == IDYES)
                {
                    pFocusWorldManager->DestroyWorld(name);
                }
            }
        }
        ImGui::EndMenu();
    }
}
