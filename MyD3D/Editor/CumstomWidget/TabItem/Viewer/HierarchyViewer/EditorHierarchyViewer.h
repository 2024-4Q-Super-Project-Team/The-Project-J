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
        // �����ϴ� �ν����� â.
        // Ȱ��ȭ ������Ʈ�� �������ֱ� ����.
        InspectorViewer* mRefInspector;
        // ������ ����Ŵ���
        // �ش� ����Ŵ����� �ִ� ������Ʈ�� ������.
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
