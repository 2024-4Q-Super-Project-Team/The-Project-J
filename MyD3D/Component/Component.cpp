#include "pch.h"
#include "Component.h"

Component::Component(Object* _owner)
	: gameObject(_owner)
    , isActive(true)
{
}

Component::~Component()
{
}

void Component::EditorRendering()
{
	for (auto serial : mSerials)
	{
		serial->widget->Render();
	}
}
