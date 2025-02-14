#pragma once
#include "Editor/Interface/IEditorObject.h"

class WorldManager;
class Object;
class Animator;

namespace Editor
{
    class InspectorViewer;

    class HierarchyViewer : public IWidget
    {
    public:
        explicit HierarchyViewer();
        virtual ~HierarchyViewer() = default;
    public:
        virtual void Render() override;
    public:
        void SetFocusInspector(InspectorViewer* _pInspector);
        void SetFocusWorldManager(WorldManager* _pWorldManager);
    private:
        void RenderWorld(World* _pWorld);
        void RenderObject(Object* _pObject);
    public:
        inline auto GetFocusInpectorViwer() { return mRefInspector; }
        inline auto GetFocusWorldManager()  { return mRefWorldManager; }
    private:
        // 참조하는 인스펙터 창.
        // 활성화 오브젝트를 변경해주기 위함.
        InspectorViewer* mRefInspector;
        // 참조할 월드매니저
        // 해당 월드매니저에 있는 오브젝트를 나열함.
        WorldManager* mRefWorldManager;
    public: 
        /////////////////////////////////////////////
        // World Popup
        /////////////////////////////////////////////
        bool isAddObjectPopupOpen = false;
        void ShowAddObjectPopup();
        /////////////////////////////////////////////
        // Object Popup
        /////////////////////////////////////////////
        Object* mPopupObject = nullptr;
        bool isDeleteObjectPopupOpen = false;
        void ShowDeleteObjectPopup();

    };
}
