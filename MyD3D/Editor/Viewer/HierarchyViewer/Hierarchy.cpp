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

                {   // ���� �̸� ���
                    std::string worldName;
                    worldName.assign(pCurWorld->GetName().begin(), pCurWorld->GetName().end());
                    ImGui::Text(worldName.c_str());
                }

                //������Ʈ �׷� �߰� ��ư
                {
                    ImGui::SameLine();
                    bool buttonClicked = ImGui::Button("AddObjectGroup");

                    if (buttonClicked)
                        mbAddingGroup = true;
                    if (mbAddingGroup)
                    {
                        ImGui::SameLine();
                        char nameBuffer[20] = ""; //������Ʈ �׷� �̸� �Է�
                        if (ImGui::InputText("GroupName", nameBuffer, 20, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            mbAddingGroup = false;

                            wchar_t wcNameBuffer[20] = L"";
                            MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                            //������Ʈ �׷� ����
                            AddObjectGroup(wcNameBuffer);
                        }
                    }
                }

                auto groups = pCurWorld->GetObjectGroups();
                for (int i =0 ; i<groups.size(); i++)
                {
                    auto group = groups[i];

                    std::string name;
                    {   // �׷� �̸� ��ȯ
                        name.assign(group->GetName().begin(), group->GetName().end());
                        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                    }

                    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
                    {
                        //������Ʈ �߰� ��ư
                        {
                            ImGui::SameLine();
                            std::string objBtnName = "AddObject##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                            bool buttonClicked = ImGui::Button(objBtnName.c_str());

                            if (buttonClicked)
                                mbAddingObjIndex = i;

                            if (mbAddingObjIndex == i)
                            {
                                ImGui::SameLine();
                                char nameBuffer[20] = ""; //������Ʈ �̸� �Է�
                                if (ImGui::InputText(("##" + objBtnName).c_str(), nameBuffer, 10, ImGuiInputTextFlags_EnterReturnsTrue))
                                {
                                    mbAddingObjIndex = -1;

                                    wchar_t wcNameBuffer[20] = L"";
                                    MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                                    //������Ʈ �׷� ����
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
                // ���� ��Ƽ�� ���尡 ����.
            }
        }
        else
        {
            // �����ϴ� ����Ŵ����� ����.
        }
    }

    void Hierarchy::RenderObjectGroup(ObjectGroup* _pObjectGroup)
    {
        auto& list = _pObjectGroup->GetObjects();
        for (auto& obj : list)
        {
            // �θ� ������ �ֻ�� �θ� �ȿ��� �˾Ƽ� �׷��ٰŹǷ� �н�
            if (obj->transform->GetParent()) continue;
            RenderObject(obj);
        }
    }

    void Hierarchy::RenderObject(Object* _pObject)
    {
        std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        std::string name = Helper::ToString(_pObject->GetName());

        // ���� ��Ŀ���� ��ü����
        bool isFocused = false;
        // Ʈ�� ��尡 �����ִ���
        bool isOpened = false;
        // TreeNode �÷��� ����
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (mRefInspector && mRefInspector->GetFocusObject() == _pObject)
        {
            flags |= ImGuiTreeNodeFlags_Selected; // ���� ���� �÷��� �߰�
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        isOpened = ImGui::TreeNodeEx(name.c_str(), flags);
        ImGui::PopStyleColor(2);

        // Ŭ�� �̺�Ʈ ����
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

