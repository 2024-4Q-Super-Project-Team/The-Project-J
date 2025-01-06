#pragma once
#include "Component/EditorUI/Contents/EditorWidget.h"
#include "Component/EditorUI/Contents/EditorOptional.h"
#include "Component/EditorUI/Contents/EditorUID.h"

namespace Editor
{
    class Menu;
    class MenuItem;

    class MenuBar : public Widget
    {
    public:
        explicit MenuBar() = default;
        virtual ~MenuBar() = default;
    public:
        Menu* AddMenu(const char* _mainLabel);
    public:
        virtual void Render() override;
    private:
        std::vector<Menu*> mContainer;
    };


    class Menu : public Widget, public EUID
    {
    public:
        explicit Menu(const char* _mainLabel);
        virtual ~Menu() = default;
    public:
        MenuItem*   AddItem(const char* _mainLabel, const char* _subLabel = nullptr, std::function<void()> action = nullptr);
    public:
        inline void SetMainLabel(const char* _label) { ReSetUID(_label); }
        inline void SetActive(bool _bActive) { isActive = _bActive; }
    public:
        virtual void Render() override;
    private:
        bool        isActive;
        std::vector<MenuItem*>  mContainer;
    };


    class MenuItem : public Widget, public EUID
    {
    public:
        explicit MenuItem(const char* _mainLabel, const char* _subLabel, std::function<void()> action = nullptr);
        virtual ~MenuItem() = default;
    public:
        inline void SetMainLabel(const char* _label) { ReSetUID(_label); }
        inline void SetSubLabel(const char* _label) { mSubLabel = _label; }
        inline void SetActive(bool _bActive) { isActive = _bActive; }
        inline void SetAction(std::function<void()> _func) { mAction = _func; }
    public:
        virtual void Render() override;
    private:
        const char* mSubLabel;
        bool        isActive;
        std::function<void()> mAction;
    };
}

