#include "pch.h"
#include "Hierarchy.h"
#include "Editor/Viewer/InspectorViewer/Inspector.h"
#include "Editor/Viewer/ResourceViewer/EditorResourceView.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

namespace Editor
{
    Hierarchy::Hierarchy()
        : mRefInspector(nullptr)
        , mRefWorldManager(nullptr)
    {
    }

    void Hierarchy::Render()
    {
        if (mRefWorldManager)
        {
            World* pCurWorld = mRefWorldManager->GetActiveWorld();
            if (pCurWorld)
            {
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
                for (int i =0 ; i<groups.size(); i++)
                {
                    auto group = groups[i];

                    std::string name;
                    {   // 그룹 이름 변환
                        name.assign(group->GetName().begin(), group->GetName().end());
                        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                    }

                    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
                    {
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
                                if (ImGui::InputText(("##" + objBtnName).c_str(), nameBuffer, 10, ImGuiInputTextFlags_EnterReturnsTrue))
                                {
                                    mbAddingObjIndex = -1;

                                    wchar_t wcNameBuffer[20] = L"";
                                    MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                                    //오브젝트 그룹 생성
                                    AddObject(wcNameBuffer, group);
                                }
                            }
                        }
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

    void Hierarchy::RenderObjectGroup(ObjectGroup* _pObjectGroup)
    {
        auto& list = _pObjectGroup->GetObjects();
        for (auto& obj : list)
        {
            // 부모가 있으면 최상단 부모가 안에서 알아서 그려줄거므로 패스
            if (obj->transform->GetParent()) continue;
            RenderObject(obj);
        }
    }

    void Hierarchy::RenderObject(Object* _pObject)
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        std::string name = Helper::ToString(_pObject->GetName());

        // 현재 포커스된 객체인지
        bool isFocused = false;
        // 트리 노드가 열려있는지
        bool isOpened = false;
        // TreeNode 플래그 설정
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (mRefInspector && mRefInspector->GetFocusObject() == _pObject)
        {
            flags |= ImGuiTreeNodeFlags_Selected; // 선택 상태 플래그 추가
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        isOpened = ImGui::TreeNodeEx(name.c_str(), flags);
        ImGui::PopStyleColor(2);

        // 클릭 이벤트 감지
        if (mRefInspector && ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            mRefInspector->SetFocusObject(_pObject);
        }
        if (isOpened)
        {
            const std::vector<Transform*>& children = _pObject->transform->GetChildren();
            for (auto child : children)
            {
                RenderObject(child->gameObject);
            }
            ImGui::TreePop();
        }
    }
    void Hierarchy::AddObjectGroup(const std::wstring _name)
    {
        mRefWorldManager->GetActiveWorld()->CreateObjectGroup(std::wstring_view(_name));
    }
    void Hierarchy::AddObject(const std::wstring _name, ObjectGroup* _group)
    {
        _group->CreateObject(_name, L"");
    }
    void Hierarchy::SaveWorld()
    {
        mRefWorldManager->SaveWorlds();
    }
    void Hierarchy::SetFocusInspector(Inspector* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void Hierarchy::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
    }
}

