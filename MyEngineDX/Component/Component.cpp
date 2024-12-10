#include "pch.h"
#include "Component.h"

Component::ComponentBase::ComponentBase(GameObject* _owner)
	: ownerObject(_owner) 
	, isActive(true)
	, mType(ComponentType::SIZE)
{
}
