#include "pch.h"
#include "Hierarchy.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Object/Object.h"

namespace Editor
{
    Hierarchy::Hierarchy()
        : EUID("Hierarchy")
        , mRefInspector(nullptr)
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
                {   // 월드 이름 출력
                    std::string worldName;
                    worldName.assign(pCurWorld->GetName().begin(), pCurWorld->GetName().end());
                    ImGui::Text(worldName.c_str());
                }
                auto groups = pCurWorld->GetObjectGroups();
                for (auto& group : groups)
                {
                    std::string name;
                    {   // 그룹 이름 변환
                        name.assign(group->GetName().begin(), group->GetName().end());
                        name += "##" + std::to_string(reinterpret_cast<uintptr_t>(group));
                    }
                    if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
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
    void Hierarchy::SetFocusInspector(Inspector* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void Hierarchy::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
    }
}

