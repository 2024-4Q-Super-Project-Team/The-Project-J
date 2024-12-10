#include "pch.h"
#include "Script.h"

namespace Component
{
	Script::Script(GameObject* _owner) : ComponentBase(_owner)
	{
		mType = ComponentType::Script;
	}
}