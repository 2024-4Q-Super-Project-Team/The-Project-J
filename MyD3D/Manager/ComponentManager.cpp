#include "pch.h"
#include "ComponentManager.h"

bool ComponentManager::Initialize()
{
	REGISTER_COMPONENT(Animator);
	REGISTER_COMPONENT(Camera);
	REGISTER_COMPONENT(Light);
	REGISTER_COMPONENT(MeshRenderer);
	REGISTER_COMPONENT(SkinnedMeshRenderer);
	REGISTER_COMPONENT(AudioSource);
	REGISTER_COMPONENT(AudioListener);
	REGISTER_COMPONENT(Rigidbody);
	REGISTER_COMPONENT(BoxCollider);
	REGISTER_COMPONENT(SphereCollider);
	REGISTER_COMPONENT(PlayerController);


	return true;
}

void ComponentManager::Run()
{
}

void ComponentManager::Finalization()
{
}