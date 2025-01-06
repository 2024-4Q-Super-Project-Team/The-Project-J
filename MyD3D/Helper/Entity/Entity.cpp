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
			// Create���� Active�� �ٲ� �� �ݹ��� ȣ���ϸ� �ȵȴ�. 
			// �׷��Ƿ� ���� ���°� Passive�϶��� ȣ���ϵ��� ������ ����
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
