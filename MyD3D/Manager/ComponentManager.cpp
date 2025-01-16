#include "pch.h"
#include "ComponentManager.h"

bool ComponentManager::Initialize()
{
	REGISTER_COMPONENT(Animator);
	REGISTER_COMPONENT(Camera);
	REGISTER_COMPONENT(Light);
	REGISTER_COMPONENT(MeshRenderer);
	REGISTER_COMPONENT(SkinnedMeshRenderer);


	return true;
}

void ComponentManager::Run()
{
}

void ComponentManager::Finalization()
{
}