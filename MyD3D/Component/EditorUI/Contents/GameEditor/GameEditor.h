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
        // 참조하는 인스펙터 창.
        // 활성화 오브젝트를 변경해주기 위함.
        Inspector* mRefInspector;
        // 참조할 월드매니저
        // 해당 월드매니저에 있는 오브젝트를 나열함.
        WorldManager* mRefWorldManager;

        bool mbAddingGroup = false;
        int mbAddingObjIndex = -1;
    };
}