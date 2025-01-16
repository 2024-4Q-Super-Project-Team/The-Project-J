#pragma once
#include "Editor/Interface/IEditorObject.h"

class WorldManager;
class Object;
class ObjectGroup;

namespace Editor
{
    class Inspector;

    class Hierarchy : public IWidget
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

        void AddObjectGroup(const std::wstring _name);
        void AddObject(const std::wstring _name, ObjectGroup* _group);

        void SaveWorld();
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
