#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorUID.h"

class WorldManager;
class Object;
class ObjectGroup;

namespace Editor
{
    class Inspector;

    class Hierarchy : public Widget, public EUID
    {
    public:
        explicit Hierarchy();
        virtual ~Hierarchy() = default;
    public:
        virtual void Render() override;
    public:
        void SetFocusInspector(Inspector* _pInspector);
        void SetFocusWorldManager(WorldManager* _pWorldManager);
    private:
        void RenderObjectGroup(ObjectGroup* _pObjectGroup);
        void RenderObject(Object* _pObject);
    private:
        // �����ϴ� �ν����� â.
        // Ȱ��ȭ ������Ʈ�� �������ֱ� ����.
        Inspector* mRefInspector;
        // ������ ����Ŵ���
        // �ش� ����Ŵ����� �ִ� ������Ʈ�� ������.
        WorldManager* mRefWorldManager;
    };
}
