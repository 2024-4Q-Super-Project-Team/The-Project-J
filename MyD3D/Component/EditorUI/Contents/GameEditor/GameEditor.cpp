#include "pch.h"
#include "GameEditor.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

#include <fstream>

namespace Editor
{
	GameEditor::GameEditor()
		: EUID("GameEditor"), mRefInspector(nullptr), mRefWorldManager(nullptr)
	{
	}

	void GameEditor::Render()
	{
        if (mRefWorldManager)
        {
            World* pCurWorld = mRefWorldManager->GetActiveWorld();
            if (pCurWorld)
            {
                ImGui::SameLine();
                bool saveClicked = ImGui::Button("Save");

                if (saveClicked)
                {
                    SaveWorld();
                }

                {   // 월드 이름 출력
                    std::string worldName;
                    worldName.assign(pCurWorld->GetName().begin(), pCurWorld->GetName().end());
                    ImGui::Text(worldName.c_str());
                }

                //오브젝트 그룹 추가 버튼
                {
                    ImGui::SameLine();
                    bool buttonClicked = ImGui::Button("AddObjectGroup");

                    if (buttonClicked)
                        mbAddingGroup = true;
                    if (mbAddingGroup)
                    {
                        ImGui::SameLine();
                        char nameBuffer[20] = ""; //오브젝트 그룹 이름 입력
                        if (ImGui::InputText("GroupName", nameBuffer, 20, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            mbAddingGroup = false;

                            wchar_t wcNameBuffer[20] = L"";
                            MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                            //오브젝트 그룹 생성
                            AddObjectGroup(wcNameBuffer);
                        }
                    }
                }

                auto groups = pCurWorld->GetObjectGroups();
                for (int i = 0; i < groups.size(); ++i)
                {
                    auto group = groups[i];

                    std::string name;
                    {   // 그룹 이름 변환
                        name.assign(group->GetName().begin(), group->GetName().end());
                        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                    }
                    bool bGroupOpend = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

                    //오브젝트 추가 버튼
                    {
                        ImGui::SameLine();
                        std::string objBtnName = "AddObject##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                        bool buttonClicked = ImGui::Button(objBtnName.c_str());

                        if (buttonClicked)
                            mbAddingObjIndex = i;

                        if (mbAddingObjIndex == i)
                        {
                            ImGui::SameLine();
                            char nameBuffer[20] = ""; //오브젝트 이름 입력
                            if (ImGui::InputText(("txt" + objBtnName).c_str(), nameBuffer, 10, ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                mbAddingObjIndex = -1;

                                wchar_t wcNameBuffer[20] = L"";
                                MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                                //오브젝트 그룹 생성
                                AddObject(wcNameBuffer, group);
                            }
                        }
                    }


                    if (bGroupOpend)
                    {
                        RenderObjectGroup(group);
                        ImGui::TreePop();
                    }
 
                }
            }
            else
            {
                // 현재 액티브 월드가 없다.
            }
        }
        else
        {
            // 참조하는 월드매니저가 없다.
        }
	}


    void GameEditor::RenderObjectGroup(ObjectGroup* _pObjectGroup)
    {
        auto& list = _pObjectGroup->GetObjects();
        for (auto& obj : list)
        {
            // 부모가 있으면 최상단 부모가 안에서 알아서 그려줄거므로 패스
            if (obj->transform->GetParent()) continue;
            RenderObject(obj);
        }
    }

    void GameEditor::RenderObject(Object* _pObject)
    {
        std::string name;
        name.assign(_pObject->GetName().begin(), _pObject->GetName().end());

        // 고유 ID 추가 (Object의 포인터 사용)
        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(_pObject));

        // 현재 포커스된 객체인지 확인
        bool isFocused = false;
        if (mRefInspector)
            isFocused = (mRefInspector->GetFocusObject() == _pObject);

        // TreeNode 플래그 설정
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (isFocused)
        {
            flags |= ImGuiTreeNodeFlags_Selected; // 선택 상태 플래그 추가
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        bool isOpened = ImGui::TreeNodeEx(name.c_str(), flags);
        ImGui::PopStyleColor(2);

        // 클릭 이벤트 감지
        if (ImGui::IsItemClicked())
        {
            if (mRefInspector)
                mRefInspector->SetFocusObject(_pObject);
        }

        // TreeNode가 열렸을 경우 자식 노드 순회
        if (isOpened)
        {
            auto children = _pObject->transform->GetChildren();
            for (auto child : children)
            {
                RenderObject(child->gameObject);
            }
            ImGui::TreePop();
        }
    }
    void GameEditor::AddObjectGroup(const std::wstring _name)
    {
        mRefWorldManager->GetActiveWorld()->CreateObjectGroup(std::wstring_view(_name));
    }
    void GameEditor::AddObject(const std::wstring _name, ObjectGroup* _group)
    {
        _group->CreateObject(_name, L"");
    }

    void GameEditor::SaveWorld()
    {
        World* pCurWorld = mRefWorldManager->GetActiveWorld();
        json data = pCurWorld->Serialize();

        std::ofstream file("./save.json");
        if(file.is_open())
            file << data.dump(4);

        file.close();
    }
 
    void GameEditor::SetFocusInspector(Inspector* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void GameEditor::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
    }
}