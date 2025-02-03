#include "pch.h"
#include "EditorHierarchyViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/InspectorViewer/EditorInspectorViewer.h"
#include "Editor/CumstomWidget/TabItem/Viewer/ResourceViewer/EditorResourceViewer.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Object/Object.h"


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
            World* pStartWorld = mRefWorldManager->GetStartWorld();
            ImGui::Text("Start World: "); ImGui::SameLine();
            if (pStartWorld)
            {
                ImGui::Text(Helper::ToString(pStartWorld->GetName()).c_str());
                ImGui::Separator();
            }
            else
                ImGui::Text("null");


            World* pCurWorld = mRefWorldManager->GetActiveWorld();
            ImGui::Text("Focus World");
            RenderWorld(pCurWorld);
            ImGui::Separator();

            ImGui::Text("Persistance World");
            for (auto& world : mRefWorldManager->GetWorlds())
            {
                if(world->IsPersistance())
                    RenderWorld(world);
            }
            ImGui::Separator();

            if (ImGui::TreeNodeEx("World Viewer"))
            {
                for (auto& world : mRefWorldManager->GetWorlds())
                {
                    RenderWorld(world);
                }
                ImGui::TreePop();
            }
        }
        ShowAddObjectPopup();
        ShowDeleteObjectPopup();
    }

    void HierarchyViewer::RenderWorld(World* _pWorld)
    {
        std::string widgetID = "Null Focus World";
        if (_pWorld)
        {
            std::string name = Helper::ToString(_pWorld->GetName());
            std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(_pWorld));
            widgetID = name + ptr;
            {
                _pWorld->EditorRendering(EditorViewerType::DEFAULT);
                // 오른쪽 클릭을 감지하고 팝업 메뉴를 열기
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    ImGui::OpenPopup(ptr.c_str()); // 유니크한 식별자로 팝업을 엶
                }
                // 팝업 창
                if (ImGui::BeginPopup(ptr.c_str()))
                {
                    if (ImGui::MenuItem("Add Object")) {
                        isAddObjectPopupOpen = true;
                        mPopupObject = nullptr;
                    }
                    ImGui::EndPopup();
                }
                Object* receiveObject = nullptr;
                if (EditorDragNDrop::ReceiveDragAndDropObjectData((widgetID).c_str(), &receiveObject))
                {
                    receiveObject->transform->SetParent(nullptr);
                    receiveObject->SetWorld(_pWorld);
                }
            }
            ////////////////////////////////////////////////////////////////////////
            // ObjectRender
            ////////////////////////////////////////////////////////////////////////
            if (_pWorld->isNodeOpen || mRefWorldManager->GetActiveWorld() == _pWorld)
            {
                ImGui::Indent(); // 한 칸 들여쓰기
                {
                    auto& ObjectArr = _pWorld->GetObjectArray();
                    for (int i = 0; i < ObjectArr.size(); i++)
                    {
                        if (ObjectArr[i]->transform->GetParent())
                            continue;
                        RenderObject(ObjectArr[i]);
                    }
                }
                ImGui::Unindent(); // 들여쓰기 원래 상태로 되돌리기
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
            ImGui::Selectable((widgetID).c_str(), false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
        }
      
    }

    void HierarchyViewer::RenderObject(Object* _pObject)
    {
        if (_pObject)
        {
            std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(_pObject));

            _pObject->EditorRendering(EditorViewerType::DEFAULT);

            // 오른쪽 클릭 - 팝업 메뉴 오픈
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(ptr.c_str()); // 유니크한 식별자로 팝업을 엶
            }
            if (ImGui::BeginPopup(ptr.c_str()))
            {
                if (ImGui::MenuItem("Add Child Object")) {
                    isAddObjectPopupOpen = true;
                    mPopupObject = _pObject;
                }
                if (ImGui::MenuItem("Delete Object")) {
                    isDeleteObjectPopupOpen = true;
                    mPopupObject = _pObject;
                }
                ImGui::EndPopup();
            }
            if (_pObject->isNodeOpen)
            {
                const std::vector<Transform*>& children = _pObject->transform->GetChildren();
                for (auto child : children)
                {
                    ImGui::Indent(); // 한 칸 들여쓰기
                    RenderObject(child->gameObject);
                    ImGui::Unindent(); // 들여쓰기 원래 상태로 되돌리기
                }
            }
        }
    }
    void HierarchyViewer::SetFocusInspector(InspectorViewer* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void HierarchyViewer::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
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
                if (mRefWorldManager->GetActiveWorld())
                {
                    if (mPopupObject)
                    {
                        Object* instance = mRefWorldManager->GetActiveWorld()->CreateObject(Helper::ToWString(newName), Helper::ToWString(newTag));
                        instance->transform->SetParent(mPopupObject->transform);
                    }
                    else
                    {
                        mRefWorldManager->GetActiveWorld()->CreateObject(Helper::ToWString(newName), Helper::ToWString(newTag));
                    }
                }
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
                mPopupObject = nullptr;
            }
            ImGui::SameLine();
            if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
            {
                ImGui::CloseCurrentPopup();
                strcpy_s(Name, defaultName);
                strcpy_s(Tag, defaultTag);
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

