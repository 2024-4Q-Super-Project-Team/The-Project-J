#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"


class WorldManager;
class Object;
class ObjectGroup;

namespace Editor
{
    class Inspector;

    class GameEditor : public Widget, public EUID
    {
    public:
        explicit GameEditor();
        virtual ~GameEditor() = default;
    public:
        virtual void Render() override;
    public:
        void SetFocusInspector(Inspector* _pInspector);
        void SetFocusWorldManager(WorldManager* _pWorldManager);
    private:
        void RenderObjectGroup(ObjectGroup* _pObjectGroup);
        void RenderObject(Object* _pObject);

        void AddObjectGroup(const std::wstring _name);
        void AddObject(const std::wstring _name, ObjectGroup* _group);
    private:
        // �����ϴ� �ν����� â.
        // Ȱ��ȭ ������Ʈ�� �������ֱ� ����.
        Inspector* mRefInspector;
        // ������ ����Ŵ���
        // �ش� ����Ŵ����� �ִ� ������Ʈ�� ������.
        WorldManager* mRefWorldManager;

        bool mbAddingGroup = false;
        int mbAddingObjIndex = -1;
    };
}