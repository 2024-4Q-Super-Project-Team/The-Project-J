#pragma once

enum class EditorViewerType
{
    DEFAULT,
    HIERARCHY,
    INSPECTOR,
};


class IEditorObject
{
public:
    virtual void EditorRendering(EditorViewerType _viewerType = EditorViewerType::DEFAULT) = 0;
protected:
    inline void SetID(const std::string& _label) {
        mUid = _label;
    }
public:
    inline const char* GetID() {
        return mUid.c_str();
    }
protected:
    std::string mUid;
};

namespace Editor
{
    class IWidget
    {
    public:
        virtual void Render() = 0;
    protected:
    };
}

#define EDITOR_FLAG_MAIN ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen
#define EDITOR_FLAG_RESOURCE  ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Bullet


#define EDITOR_COLOR_EXTRA		ImVec4(0.4f, 0.4f, 0.4f, 1.0f)
#define EDITOR_COLOR_RESOURCE	ImVec4(0.0f, 0.4f, 0.2f, 1.0f)
#define EDITOR_COLOR_NULL	    ImVec4(0.3f, 0.0f, 0.0f, 1.0f)
#define EDITOR_COLOR_POP(num)	ImGui::PopStyleColor(num);

