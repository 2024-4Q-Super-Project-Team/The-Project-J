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
        MenuNode* AddMenuNode(const char* _mainLabel);
    public:
        virtual void Render() override;
    private:
        std::vector<MenuNode*> mContainer;
    };


    class MenuNode : public IWidget
    {
    public:
        explicit MenuNode(const char* _label);
        virtual ~MenuNode() = default;
    public:
        MenuItem* AddMenuItem(const char* _label, std::function<void()> action = nullptr);
    public:
        inline void SetActive(bool _bActive) { isActive = _bActive; }
    public:
        virtual void Render() override;
    private:
        const char* mLabel;
        bool        isActive;
        std::vector<MenuItem*>  mContainer;
    };


    class MenuItem : public IWidget
    {
    public:
        explicit MenuItem(const char* _mainLabel, std::function<void()> action = nullptr);
        virtual ~MenuItem() = default;
    public:
        inline void SetActive(bool _bActive) { isActive = _bActive; }
        inline void SetAction(std::function<void()> _func) { mAction = _func; }
    public:
        virtual void Render() override;
    private:
        const char* mLabel;
        bool        isActive;
        std::function<void()> mAction;
    };
}

