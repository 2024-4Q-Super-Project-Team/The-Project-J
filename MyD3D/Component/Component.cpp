#include "pch.h"
#include "Component.h"
#include "Manager/GameManager.h"
Component::Component(Object* _owner)
	: gameObject(_owner)
    , isActive(true)
{
}

Component::~Component()
{
}

void Component::EditorRendering(EditorViewerType _viewerType)
{

}
