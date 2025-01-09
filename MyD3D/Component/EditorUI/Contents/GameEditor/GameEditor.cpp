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
                for (int i = 0; i < groups.size(); ++i)
                {
                    auto group = groups[i];

                    std::string name;
                    {   // �׷� �̸� ��ȯ
                        name.assign(group->GetName().begin(), group->GetName().end());
                        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                    }
                    bool bGroupOpend = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

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
                            if (ImGui::InputText(("txt" + objBtnName).c_str(), nameBuffer, 10, ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                mbAddingObjIndex = -1;

                                wchar_t wcNameBuffer[20] = L"";
                                MultiByteToWideChar(CP_UTF8, 0, nameBuffer, -1, wcNameBuffer, 20);

                                //������Ʈ �׷� ����
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
                // ���� ��Ƽ�� ���尡 ����.
            }
        }
        else
        {
            // �����ϴ� ����Ŵ����� ����.
        }
	}


    void GameEditor::RenderObjectGroup(ObjectGroup* _pObjectGroup)
    {
        auto& list = _pObjectGroup->GetObjects();
        for (auto& obj : list)
        {
            // �θ� ������ �ֻ�� �θ� �ȿ��� �˾Ƽ� �׷��ٰŹǷ� �н�
            if (obj->transform->GetParent()) continue;
            RenderObject(obj);
        }
    }

    void GameEditor::RenderObject(Object* _pObject)
    {
        std::string name;
        name.assign(_pObject->GetName().begin(), _pObject->GetName().end());

        // ���� ID �߰� (Object�� ������ ���)
        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(_pObject));

        // ���� ��Ŀ���� ��ü���� Ȯ��
        bool isFocused = false;
        if (mRefInspector)
            isFocused = (mRefInspector->GetFocusObject() == _pObject);

        // TreeNode �÷��� ����
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (isFocused)
        {
            flags |= ImGuiTreeNodeFlags_Selected; // ���� ���� �÷��� �߰�
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        bool isOpened = ImGui::TreeNodeEx(name.c_str(), flags);
        ImGui::PopStyleColor(2);

        // Ŭ�� �̺�Ʈ ����
        if (ImGui::IsItemClicked())
        {
            if (mRefInspector)
                mRefInspector->SetFocusObject(_pObject);
        }

        // TreeNode�� ������ ��� �ڽ� ��� ��ȸ
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