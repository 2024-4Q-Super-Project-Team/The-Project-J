#pragma once
#include "Editor/Interface/IEditorObject.h"

namespace Editor
{
    class MenuNode;
    class MenuItem;

    class MenuBar : public IWidget
    {
    public:
        explicit MenuBar() = default;
        virtual ~MenuBar() = default;
    public:
        void AddMenuNode(IWidget* _pWidget);
    public:
        virtual void Render() override;
    private:
        std::vector<IWidget*> mContainer;
    };


    class MenuNode : public IWidget
    {
    public:
        explicit MenuNode(const char* _label);
        virtual ~MenuNode() = default;
    public:
        void AddMenuItem(IWidget* _pWidget);
    public:
        virtual void Render() override;
    private:
        const char* mLabel;
        std::vector<IWidget*>  mContainer;
    };


    class MenuItem : public IWidget
    {
    public:
        explicit MenuItem(const char* _mainLabel, std::function<void()> action = nullptr);
        virtual ~MenuItem() = default;
    public:
        void AddSubMenuItem(IWidget* _pWidget);
        inline void SetActive(bool _bActive) { isActive = _bActive; }
        inline void SetAction(std::function<void()> _func) { mAction = _func; }
    public:
        virtual void Render() override;
    private:
        const char* mLabel;
        bool        isActive;
        std::function<void()> mAction;
        std::vector<IWidget*>  mContainer;
    };
}

