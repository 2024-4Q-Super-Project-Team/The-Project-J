#include "pch.h"
#include "Component.h"
#include "Object/Object.h"
#include "ObjectGroup/ObjectGroup.h"

Component::ComponentBase::ComponentBase(GameObject* _owner)
	: ownerObject(_owner) 
	, isActive(true)
	, mType(ComponentType::SIZE)
{
}

DXWorld* Component::ComponentBase::GetWorld() const
{
	return ownerObject->GetOwnerGroup()->GetWorld();
}
