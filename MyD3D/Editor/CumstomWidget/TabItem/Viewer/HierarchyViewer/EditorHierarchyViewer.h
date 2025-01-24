#pragma once
#include "Editor/Interface/IEditorObject.h"

class WorldManager;
class Object;
class ObjectGroup;

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
        void RenderObjectGroup(ObjectGroup* _pObjectGroup);
        void RenderObject(Object* _pObject, ObjectGroup* _pObjectGroup);

        void AddObjectGroup(const std::wstring _name);
        void AddObject(const std::wstring _name, ObjectGroup* _group);

        void SaveWorld();
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

        bool mbAddingGroup = false;
        int mbAddingObjIndex = -1;
    public: 
        /////////////////////////////////////////////
        // ObjectGroup Popup
        /////////////////////////////////////////////
        ObjectGroup* mPopupGroup = nullptr;
        bool isAddObjectPopupOpen = false;
        bool isDeleteGroupPopupOpen = false;
        bool isRenameGroupPopupOpen = false;
        void ShowAddObjectPopup();
        void ShowDeleteGroupPopup();
        void ShowRenameGroupPopup();
        /////////////////////////////////////////////
        // Object Popup
        /////////////////////////////////////////////
        Object* mPopupObject = nullptr;
        bool isAddChildPopupOpen = false;
        bool isDeleteObjectPopupOpen = false;
        void ShowAddChildObjectPopup();
        void ShowDeleteObjectPopup();
    };
}
