#include "pch.h"
#include "Entity.h"

void Engine::Entity::SetActive(bool _isActive)
{
	EntityState preState = mState;
	if (preState != EntityState::Destroy)
	{
		if (_isActive)
		{
			mState = EntityState::Active;
			if (preState == EntityState::Passive)
				OnEnable();
		}
		if (!_isActive)
		{
			mState = EntityState::Passive;
			if (preState == EntityState::Active)
				OnDisable();
		}
	}
}

void Engine::Entity::SetDestroy()
{
	if (mState != EntityState::Create)
	{
		mState = EntityState::Destroy;
		OnDestroy();
	}
}