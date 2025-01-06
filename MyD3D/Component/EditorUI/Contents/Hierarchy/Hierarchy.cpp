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
                {   // ���� �̸� ���
                    std::string worldName;
                    worldName.assign(pCurWorld->GetName().begin(), pCurWorld->GetName().end());
                    ImGui::Text(worldName.c_str());
                }
                auto groups = pCurWorld->GetObjectGroups();
                for (auto& group : groups)
                {
                    std::string name;
                    {   // �׷� �̸� ��ȯ
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
    void Hierarchy::SetFocusInspector(Inspector* _pInspector)
    {
        mRefInspector = _pInspector;
    }
    void Hierarchy::SetFocusWorldManager(WorldManager* _pWorldManager)
    {
        mRefWorldManager = _pWorldManager;
    }
}

