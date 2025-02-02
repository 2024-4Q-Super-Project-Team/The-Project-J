#include "pch.h"
#include "Entity.h"
#include "Manager/GameManager.h"

void Engine::Entity::SetActive(bool _isActive)
{
	EntityState preState = mState;
	if (preState != EntityState::Destroy)
	{
		if (_isActive)
		{
			mState = EntityState::Active;
			// Create에서 Active로 바뀔 땐 콜백을 호출하면 안된다. 
			// 그러므로 이전 상태가 Passive일때만 호출하도록 조건을 설정
			if (preState == EntityState::Passive && GameManager::GetRunType() == eEngineRunType::GAME_MODE)
				OnEnable();
		}
		if (!_isActive)
		{
			mState = EntityState::Passive;
			if (preState == EntityState::Active && GameManager::GetRunType() == eEngineRunType::GAME_MODE)
				OnDisable();
		}
	}
}

void Engine::Entity::SetDestroy()
{
	// 여러번 OnDestory를 호출하면 안되니까
	if (mState != EntityState::Destroy)
	{
		mState = EntityState::Destroy;
		OnDestroy();
	}
}

void Engine::Entity::SetCreate()
{
	// Create상태가 아니면 콜백함수를 호출할 필요가 없다. Reset할 때말곤 안쓰기 때문
	if (mState != EntityState::Create)
	{
		// 따라서 Create상태로만 바꿔준다.
		mState = EntityState::Create;
	}
	// 근데 처음 만들어진거면 어차피 Create상태이다. 따라서 콜백을 호출해준다.
	else 
	{
		OnCreate();
	}
}
