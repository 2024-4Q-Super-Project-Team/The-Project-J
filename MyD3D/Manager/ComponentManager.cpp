#include "pch.h"
#include "ComponentManager.h"

bool ComponentManager::Initialize()
{
	REGISTER_COMPONENT(Camera);
	REGISTER_COMPONENT(Light);
	REGISTER_COMPONENT(Animator);
	REGISTER_COMPONENT(BoneAttacher);
	REGISTER_COMPONENT(FiniteStateMachine);
	REGISTER_COMPONENT(MeshRenderer);
	REGISTER_COMPONENT(SkinnedMeshRenderer);
	REGISTER_COMPONENT(AudioSource);
	REGISTER_COMPONENT(AudioListener);
	REGISTER_COMPONENT(Rigidbody);
	REGISTER_COMPONENT(BoxCollider);
	REGISTER_COMPONENT(SphereCollider);
	REGISTER_COMPONENT(ConvexCollider);
	REGISTER_COMPONENT(PlayerController);
	REGISTER_COMPONENT(ParticleSystem);
	REGISTER_COMPONENT(Canvas);
	REGISTER_COMPONENT(UISprite);
	REGISTER_COMPONENT(UIText);

	return true;
}

void ComponentManager::Run()
{
}

void ComponentManager::Finalization()
{
}