#include "pch.h"
#include "EditorHierarchyViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/InspectorViewer/EditorInspectorViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/ResourceViewer/EditorResourceViewer.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"
#include "Save/SaveManager.h"

namespace Editor
{
    HierarchyViewer::HierarchyViewer()
        : mRefInspector(nullptr)
        , mRefWorldManager(nullptr)
    {
    }

    void HierarchyViewer::Render()
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

                RenderWorld(pCurWorld);

                auto groups = pCurWorld->GetObjectGroups();
                for (int i = 0; i < groups.size(); i++)
                {
                    auto group = groups[i];
                    RenderObjectGroup(group);
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
        ShowAddObjectGroupPopup();

        ShowAddObjectPopup();
        ShowDeleteGroupPopup();
        ShowRenameGroupPopup();

        ShowAddChildObjectPopup();
        ShowDeleteObjectPopup();
    }

    void HierarchyViewer::RenderWorld(World* _pWorld)
    {
        std::string name = Helper::ToString(_pWorld->GetName());
        std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(_pWorld));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_WORLD);
        if (ImGui::Selectable((name + ptr).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(_pWorld);
            }
        }
        // ������ Ŭ���� �����ϰ� �˾� �޴��� ����
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup(ptr.c_str()); // ����ũ�� �ĺ��ڷ� �˾��� ��
        }
        // �˾� â
        if (ImGui::BeginPopup(ptr.c_str()))
        {
            if (ImGui::MenuItem("Add Object Group")) {
                isAddObjectGroupPopupOpen = true;
            }
            ImGui::EndPopup();
        }
        EDITOR_COLOR_POP(1);
    }

    // ������Ʈ �׷�
    void HierarchyViewer::RenderObjectGroup(ObjectGroup* _pObjectGroup)
    {
        std::string name = Helper::ToString(_pObjectGroup->GetName());
        std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(_pObjectGroup));
        bool isSelected = ImGui::TreeNodeEx((name + ptr).c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

        // ������ Ŭ���� �����ϰ� �˾� �޴��� ����
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup(ptr.c_str()); // ����ũ�� �ĺ��ڷ� �˾��� ��
        }
        // �˾� â
        if (ImGui::BeginPopup(ptr.c_str()))
        {
            if (ImGui::MenuItem("Add Object")) {
                isAddObjectPopupOpen = true;
                mPopupGroup = _pObjectGroup;
            }
            if (ImGui::MenuItem("Delete Group")) {
                isDeleteGroupPopupOpen = true;
                mPopupGroup = _pObjectGroup;
            }
            if (ImGui::MenuItem("Rename Group")) {
                isRenameGroupPopupOpen = true;
                mPopupGroup = _pObjectGroup;
            }
            ImGui::EndPopup();
        }
        if (isSelected)
        {
            auto& list = _pObjectGroup->GetObjects();
            for (auto& obj : list)
            {
                // �θ� ������ �ֻ�� �θ� �ȿ��� �˾Ƽ� �׷��ٰŹǷ� �н�
                if (obj->transform->GetParent()) continue;
                RenderObject(obj, _pObjectGroup);
            }
            ImGui::TreePop();
        }
    }   

    void HierarchyViewer::RenderObject(Object* _pObject, ObjectGroup* _pObjectGroup)
    {
        std::string name = Helper::ToString(_pObject->GetName());
        std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(_pObject));

        // TreeNode �÷��� ����
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (mRefInspector && mRefInspector->GetFocusObject() == _pObject)
        {
            flags |= ImGuiTreeNodeFlags_Selected; // ���� ���� �÷��� �߰�
        }
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        bool isSelected = ImGui::TreeNodeEx((name + ptr).c_str(), flags);
        ImGui::PopStyleColor(1);

        // ���� Ŭ�� - �ν����� ��Ŀ��
        if (mRefInspector && ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            EditorManager::mInspectorViewer->SetFocusObject(_pObject);
            EditorManager::mGuizmoHandler->mManipulater->SetFocusObjedct(_pObject);
        }
        // ������ Ŭ�� - �˾� �޴� ����
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup(ptr.c_str()); // ����ũ�� �ĺ��ڷ� �˾��� ��
        }
        if (ImGui::BeginPopup(ptr.c_str()))
        {
            if (ImGui::MenuItem("Add Child Object")) {
                isAddChildPopupOpen = true;
                mPopupGroup = _pObjectGroup;
                mPopupObject = _pObject;
            }
            if (ImGui::MenuItem("Delete Object")) {
                isDeleteObjectPopupOpen = true;
                mPopupGroup = _pObjectGroup;
                mPopupObject = _pObject;
            }
            ImGui::EndPopup();
        }
        if (isSelected)
        {
            const std::vector<Transform*>& children = _pObject->transform->GetChildren();
            for (auto child : children)
            {
                RenderObject(child->gameObject, _pObjectGroup);
            }
            ImGui::TreePop();
        }
    }
    void HierarchyViewer::AddObjectGroup(const std::wstring _name)
    {
        mRefWorldManager->GetActiveWorld()->CreateObjectGroup(std::wstring_view(_name));
    }
    void HierarchyViewer::AddObject(const std::wstring _name, ObjectGroup* _group)
    {
        _group->CreateObject(_name, L"");
    }
    void HierarchyViewer::SaveWorld()
    {
        SaveManager::Save();
    }
    void HierarchyViewer::SetFocusInspector(InspectorViewer* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void HierarchyViewer::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
    }

    void HierarchyViewer::ShowAddObjectGroupPopup()
    {
        std::string id = "Add Object Group";
        if (isAddObjectGroupPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isAddObjectGroupPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Object Group Name : ");
            static char Name[128] = "ObjectGroup";
            ImGui::InputText("##AddObjectGroupName", Name, IM_ARRAYSIZE(Name));

            const char* defaultName = "ObjectGroup";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                std::string newName = Name;
                if (mRefWorldManager->GetActiveWorld())
                {
                    mRefWorldManager->GetActiveWorld()
                        ->CreateObjectGroup(Helper::ToWString(newName));
                }
                else
                {
                    Display::Console::Log("Failed To Create ObjectGroup, Because Not ActiveWorld");
                }
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                mPopupGroup = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                mPopupGroup = nullptr;
            }
            ImGui::EndPopup();
        }
    }

    void HierarchyViewer::ShowAddObjectPopup()
    {
        std::string id = "Add Object";
        if (isAddObjectPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isAddObjectPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Object Name : ");
            static char Name[128] = "GameObject";
            ImGui::InputText("##AddObjectName", Name, IM_ARRAYSIZE(Name));

            ImGui::Text("Object Tag : ");
            static char Tag[128] = ""; 
            ImGui::InputText("##AddObjectTag", Tag, IM_ARRAYSIZE(Tag));

            const char* defaultName = "GameObject";
            const char* defaultTag = "";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                std::string newName = Name;
                std::string newTag = Tag;
                if (mPopupGroup)
                {
                    mPopupGroup->CreateObject(Helper::ToWString(newName), Helper::ToWString(newTag));
                }
                else
                {
                    Display::Console::Log("Failed To Create Object, Because Nullptr Ref To ObjectGroup");
                }
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
                mPopupGroup = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
                mPopupGroup = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    void HierarchyViewer::ShowDeleteGroupPopup()
    {
        std::string id = "Delete Object Group";
        if (isDeleteGroupPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isDeleteGroupPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(("Delete Object Group : " + Helper::ToString(mPopupGroup->GetName())).c_str());
            
            const char* defaultSet = "";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                if (mPopupGroup && mRefWorldManager->GetActiveWorld())
                {
                    mPopupGroup->SetDestroy();
                    ImGui::CloseCurrentPopup();
                }
                mPopupGroup = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                mPopupGroup = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    void HierarchyViewer::ShowRenameGroupPopup()
    {
        std::string id = "Rename Object Group";
        if (isRenameGroupPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isRenameGroupPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Rename Object Group : ");
            static char Name[128] = ""; // �ӽ� �Է� ����
            ImGui::InputText("##RenameObjectGroup", Name, IM_ARRAYSIZE(Name));

            const char* defaultSet = "";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                if (mPopupGroup)
                {
                    mPopupGroup->SetName(Helper::ToWString(std::string(Name)));
                    strcpy_s(Name, defaultSet);
                    ImGui::CloseCurrentPopup();
                    mPopupGroup = nullptr;
                }
                mPopupGroup = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                strcpy_s(Name, defaultSet);
                ImGui::CloseCurrentPopup();
                mPopupGroup = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    void HierarchyViewer::ShowAddChildObjectPopup()
    {
        std::string id = "Add Child Object";
        if (isAddChildPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isAddChildPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Object Name : ");
            static char Name[128] = "GameObject";
            ImGui::InputText("##AddObjectName", Name, IM_ARRAYSIZE(Name));

            ImGui::Text("Object Tag : ");
            static char Tag[128] = "";
            ImGui::InputText("##AddObjectTag", Tag, IM_ARRAYSIZE(Tag));

            const char* defaultName = "GameObject";
            const char* defaultTag = "";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                std::string newName = Name;
                std::string newTag = Tag;
                if (mPopupGroup && mPopupObject)
                {
                    Object* Clone = mPopupGroup->CreateObject(Helper::ToWString(newName), Helper::ToWString(newTag));
                    Clone->transform->SetParent(mPopupObject->transform);
                }
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
                mPopupGroup = nullptr;
                mPopupObject = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
                mPopupGroup = nullptr;
                mPopupObject = nullptr;
            }
            ImGui::EndPopup();
        }
    }
    void HierarchyViewer::ShowDeleteObjectPopup()
    {
        std::string id = "Delete Object";
        if (isDeleteObjectPopupOpen)
        {
            ImGui::OpenPopup(id.c_str());
            isDeleteObjectPopupOpen = false;
        }
        if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(("Delete Object : " + Helper::ToString(mPopupObject->GetName())).c_str());

            const char* defaultSet = "";
            if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
            {
                if (mPopupObject)
                {
                    mPopupObject->SetDestroy();
                    ImGui::CloseCurrentPopup();
                }
                mPopupObject = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                mPopupObject = nullptr;
            }
            ImGui::EndPopup();
        }
    }
}

