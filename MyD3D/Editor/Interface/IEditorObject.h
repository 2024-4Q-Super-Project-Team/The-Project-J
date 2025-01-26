#pragma once

enum class EditorViewerType
{
    DEFAULT,
    HIERARCHY,
    INSPECTOR,
    GUIZMO,
};

enum class eEditorRenderResult
{
    NONE,
    DESTROY_COMPONENT,
    SIZE,
};


class IEditorObject
{
public:
    explicit IEditorObject() = default;
    explicit IEditorObject(const std::string& _label)
        : mUid(_label){ }
    virtual ~IEditorObject() = default;
public:
    virtual void EditorRendering(EditorViewerType _viewerType = EditorViewerType::DEFAULT) = 0;
protected:
    inline void SetEID(const std::string& _label) {
        mUid = _label;
    }
public:
    inline const char* GetEID() {
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

#define EDITOR_FLAG_MAIN ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_DefaultOpen
#define EDITOR_FLAG_RESOURCE  ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_Bullet

#define EDITOR_COLOR_OBJECT		                    ImVec4(0.0f, 0.4f, 0.2f, 1.0f)
#define EDITOR_COLOR_OBJECT_SELECTED	            ImVec4(0.0f, 0.6f, 0.4f, 1.0f)
#define EDITOR_COLOR_OBJECT_DISABLE	                ImVec4(0.2f, 0.2f, 0.2f, 1.0f)
#define EDITOR_COLOR_OBJECT_DISABLE_SELECTED	    ImVec4(0.3f, 0.3f, 0.3f, 1.0f)

#define EDITOR_COLOR_EXTRA		                    ImVec4(0.4f, 0.4f, 0.4f, 1.0f)
#define EDITOR_COLOR_WORLD	                        ImVec4(0.2f, 0.2f, 0.4f, 1.0f)
#define EDITOR_COLOR_WORLD_SELECTED	                ImVec4(0.4f, 0.4f, 0.8f, 1.0f)

#define EDITOR_COLOR_RESOURCE	                    ImVec4(0.0f, 0.4f, 0.2f, 1.0f)
#define EDITOR_COLOR_RESOURCE_SELECTED              ImVec4(0.0f, 0.6f, 0.4f, 1.0f)
#define EDITOR_COLOR_NULL	                        ImVec4(0.3f, 0.0f, 0.0f, 1.0f)
#define EDITOR_COLOR_NULL_SELECTED	                ImVec4(0.5f, 0.1f, 0.1f, 1.0f)

#define EDITOR_COLOR_POP(num)	                    ImGui::PopStyleColor(num);