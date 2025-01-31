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
			// Create���� Active�� �ٲ� �� �ݹ��� ȣ���ϸ� �ȵȴ�. 
			// �׷��Ƿ� ���� ���°� Passive�϶��� ȣ���ϵ��� ������ ����
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
	// ������ OnDestory�� ȣ���ϸ� �ȵǴϱ�
	if (mState != EntityState::Destroy)
	{
		mState = EntityState::Destroy;
		OnDestroy();
	}
}

void Engine::Entity::SetCreate()
{
	// Create���°� �ƴϸ� �ݹ��Լ��� ȣ���� �ʿ䰡 ����. Reset�� ������ �Ⱦ��� ����
	if (mState != EntityState::Create)
	{
		// ���� Create���·θ� �ٲ��ش�.
		mState = EntityState::Create;
	}
	// �ٵ� ó�� ��������Ÿ� ������ Create�����̴�. ���� �ݹ��� ȣ�����ش�.
	else 
	{
		OnCreate();
	}
}
