#include "pch.h"
#include "Canvas.h"

#include "Component/UI/UIWidget/UIWidget.h"
#include "Component/Camera/Camera.h"

Canvas::Canvas(Object* _owner)
	: Component(_owner)
{
	SetEID("Canvas");
	mType = eComponentType::CANVAS;
}

Canvas::~Canvas()
{

}

void Canvas::Tick()
{
}

void Canvas::FixedUpdate()
{
}

void Canvas::PreUpdate()
{
}

void Canvas::Update()
{
    PushDrawList();
}

void Canvas::PostUpdate()
{
}

void Canvas::PreRender()
{
}

void Canvas::Render()
{
}

void Canvas::Draw(Camera* _camera)
{
}

void Canvas::PostRender()
{
}

void Canvas::PushDrawList()
{
    if (mWidgets.empty())
        return;

    for (auto& widget : mWidgets)
    {
        if (widget->IsActive())
        {
            UIManager::mDrawWidgetList.push_back(widget);
        }
    }

    mWidgets.clear();
}

void Canvas::EditorUpdate()
{
    PushDrawList();
}

void Canvas::EditorGlobalUpdate()
{

}

void Canvas::EditorRender()
{

}

void Canvas::EditorRendering(EditorViewerType _viewerType)
{
    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        break;
    }
    case EditorViewerType::HIERARCHY:
    {
        break;
    }
    case EditorViewerType::INSPECTOR:
    {
        break;
    }
    default:
        break;
    }
}

void _CALLBACK Canvas::OnEnable()
{
    return void _CALLBACK();
}

void _CALLBACK Canvas::OnDisable()
{
    return void _CALLBACK();
}

void _CALLBACK Canvas::OnDestroy()
{
    return void _CALLBACK();
}

json Canvas::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "Canvas";

    return ret;
}

void Canvas::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());
}
