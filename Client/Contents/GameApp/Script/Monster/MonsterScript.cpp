#include "pch.h"
#include "MonsterScript.h"

#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Monster/ScopeScript.h"

#define MONSTER_ANIM_IDLE_A L"m001"
#define MONSTER_ANIM_WALK_A L"m002"
#define MONSTER_ANIM_FAST_WALK_A L"m003"
#define MONSTER_ANIM_RUN_A L"m004"
#define MONSTER_ANIM_HIT_A L"m005"
#define MONSTER_ANIM_GROGGY_A L"m006"

#define MONSTER_ANIM_IDLE_B L"mb001"
#define MONSTER_ANIM_ROTATE_B L"mb002"
#define MONSTER_ANIM_HIT_B L"mb003"
#define MONSTER_ANIM_GROGGY_B L"mb004"

#define MONSTER_SFX_STEP L"SFX_mon_step.mp3"
#define MONSTER_SFX_DEAD L"SFX_mon_death.mp3"

void MonsterScript::Start()
{
	gameObject->SetTag(L"Monster");

	if (gameObject->GetName() == L"Monster_A")
		mType.val = (int)eMonsterType::A;
	else
		mType.val = (int)eMonsterType::B;

	// Init Setting
	{
		// find Weakness
		Transform* root = gameObject->transform->GetParent();
		auto& root_children = root->GetChildren();
		for (Transform* child : root_children)
		{
			if (child->gameObject->GetName() == L"Scope")
			{
				m_pScope = child->gameObject;
				m_pScope->SetTag(L"Scope");
				m_pScope->transform->position = gameObject->transform->position;
			}

			auto& children = gameObject->transform->GetChildren();
			for (Transform* child : children)
			{
				if (child->gameObject->GetName() == L"Weakness")
				{
					m_pWeakness = child->gameObject;
					m_pWeakness->SetTag(L"Weakness");
					m_pWeakness->transform->scale = Vector3(30, 30, 22);
					m_pWeakness->transform->SetEulerAngles(Vector3(Degree::ToRadian(90.0f), 0.0f, 0.0f));
					m_pWeakness->AddComponent<SphereCollider>()->SetIsTrigger(true);
					m_pWeakness->AddComponent<BurnObjectScript>();

					if (mType.val == (int)eMonsterType::A)
					{
						m_pWeakness->transform->position.y = 38;
					}
					else
					{
						m_pWeakness->transform->position.y = 5;
					}
				}
			}
		}

		mFSM = eMonsterStateType::IDLE;
	}

	// Add componenet
	{	// Animator Component
		m_pAnimator = gameObject->GetComponent<Animator>();
		if (!m_pAnimator)
		{
			m_pAnimator = gameObject->AddComponent<Animator>();
		}
	}
	{	// RigidBody Component
		m_pRigidBody = gameObject->AddComponent<Rigidbody>();
		m_pRigidBody->SetIsRealStatic(false);
	}
	{	// Head Collider Component
		m_pHeadCollider = gameObject->AddComponent<BoxCollider>();

		if (mType.val == (int)eMonsterType::A)
		{
			m_pHeadCollider->SetPosition(Vector3{ 0,30,0 });
			m_pHeadCollider->SetExtents(Vector3{ 40,2,40 });
		}
		else
		{
			m_pHeadCollider->SetPosition(Vector3{ 0,50,0 });
			m_pHeadCollider->SetExtents(Vector3{ 42,2,27 });
		}
		
		m_pHeadCollider->SetIsTrigger(true);
	}
	{	// Body Collider Component
		m_pBodyCollider = gameObject->AddComponent<BoxCollider>();

		if (mType.val == (int)eMonsterType::A)
		{
			m_pBodyCollider->SetPosition(Vector3{ 0,3,0 });
			m_pBodyCollider->SetExtents(Vector3{ 28,15,28 });
		}
		else
		{
			m_pBodyCollider->SetPosition(Vector3{ 0,25,0 });
			m_pBodyCollider->SetExtents(Vector3{ 35,35,20 });
		}
	}

	mTargetPos = gameObject->transform->position;

	InitSFX();
}

void MonsterScript::Update()
{
	if (m_pScope)
	{
		auto* scope = m_pScope->GetComponent<ScopeScript>();

		if (scope)
			scope->SetMonster(gameObject);

		if ((m_pScope->transform->position - gameObject->transform->position).LengthSquared() > 40000)
			bIsScope = false;
		else
			bIsScope = true;
	}

	if (mType.val == (int)eMonsterType::A)
	{
		UpdateA();
		UpdateMonsterAnim_A();
	}
	else if (mType.val == (int)eMonsterType::B)
	{
		UpdateB();
		UpdateMonsterAnim_B();
	}

}

void MonsterScript::UpdateA()
{
	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		UpdateIdle();
		break;
	case eMonsterStateType::WALK:
		UpdateWalk();
		UpdateMonsterAngle(); 
		break;
	case eMonsterStateType::FAST_WALK:
		UpdateFastWalk();
		UpdateMonsterAngle();
		break;
	case eMonsterStateType::RUN:
		UpdateRun();
		UpdateMonsterAngle(); 
		break;
	case eMonsterStateType::HIT:
		UpdateHit();
		break;
	case eMonsterStateType::GROGGY:
		UpdateGroggy();
		break;
	case eMonsterStateType::DEAD:
		UpdateDead();
		break;
	default:
		break;
	}
}

void MonsterScript::UpdateB()
{
	
	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		UpdateIdle();
		break;
	case eMonsterStateType::WALK:
		UpdateWalk();
		UpdateMonsterAngle();
		break;
	case eMonsterStateType::ROTATE:
		UpdateRotate();
		UpdateMonsterAngle();
		break;
	case eMonsterStateType::HIT:
		UpdateHit();
		break;
	case eMonsterStateType::GROGGY:
		UpdateGroggy();
		break;
	case eMonsterStateType::DEAD:
		UpdateDead();
		break;
	}
	
}

void MonsterScript::UpdateIdle()
{

	if (mType.val == (int)eMonsterType::A)
	{
		mFSM = eMonsterStateType::WALK;
	}
	else
	{
		mFSM = eMonsterStateType::ROTATE;
	}

}

void MonsterScript::UpdateRotate()
{
	if (!m_pAnimator)
		return;

	if (m_pAnimator->IsEnd())
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateWalk()
{
	if (mType.val == (int)eMonsterType::A)
	{
		// Ÿ���� �����ϴ°�?
		if (m_pTarget)
		{
			mFSM = eMonsterStateType::FAST_WALK;
		}
	}
	
	mResetCount += Time::GetUnScaledDeltaTime();

	if (mResetCount > 5.0f)
	{
		// ������ ���� ���� ���� ���� ����� �������ֱ�
		if (bIsScope)
		{
			mTargetPos.x = Random::Range(mTargetPos.x - mRange, mTargetPos.x + mRange);
			mTargetPos.y = gameObject->transform->position.y;
			mTargetPos.z = Random::Range(mTargetPos.z - mRange, mTargetPos.z + mRange);
		}
		else
		{
			mTargetPos.x = m_pScope->transform->position.x;
			mTargetPos.y = gameObject->transform->position.y;
			mTargetPos.z = m_pScope->transform->position.z;
		}

		// ���� ���� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ� ����
		mDistance = mTargetDir.Length();
		// ���� ����
		mTargetDir.Normalize();

		mResetCount = 0.f;

	}
	else
	{
		if (mDistance > 10.f)
		{
			gameObject->transform->position += mTargetDir * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
		}
		else
		{

			if (mType.val == (int)eMonsterType::A)
			{
				if (m_pAnimator->IsEnd())
				{
					mFSM = eMonsterStateType::IDLE;
				}
			}
			else if (mType.val == (int)eMonsterType::B)
			{
				if (m_pAnimator->IsEnd())
				{
					mFSM = eMonsterStateType::IDLE;

				}
			}
		}
	}
}

void MonsterScript::UpdateFastWalk()
{
	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		mTargetPos = {	m_pTarget->transform->position.x,
						gameObject->transform->position.y, 
						m_pTarget->transform->position.z };
		// Ÿ�� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = mTargetDir.Length();
		// ���� 
		mTargetDir.Normalize();

		// �Ÿ��� ������ ���� �ݰ溸�� Ŭ �������� ��� �����ֱ�
		if (distance > mAttackDistance.val)
		{
			gameObject->transform->position += mTargetDir * mMoveSpeed.val * 1.5 * Time::GetUnScaledDeltaTime();
		}
		else // Ÿ�ٰ��� �Ÿ��� ���� ���� ����� ���� ���·� ��ȯ
		{
			mFSM = eMonsterStateType::RUN;
		}
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateRun()
{
	// Ÿ���� �����ϴ°�?
	if (m_pTarget)
	{
		// Ÿ�� ����
		mTargetPos = {	m_pTarget->transform->position.x,
								gameObject->transform->position.y, 
								m_pTarget->transform->position.z };
		// Ÿ�� ����
		mTargetDir = mTargetPos - gameObject->transform->position;
		// �Ÿ�
		float distance = mTargetDir.Length();
		// ����
		mTargetDir.Normalize();

		// �Ÿ��� ���� �ݰ溸�� ũ�ٸ� �ٽ� ���� ��ȯ
		if (distance > mAttackDistance.val)
			mFSM = eMonsterStateType::FAST_WALK;

		// �Ÿ��� ���� �ݰ溸�� �۴ٸ� ����
		// Ÿ�� ��ġ�� ������ �̵�
		gameObject->transform->position += mTargetDir * mMoveSpeed.val * 2.f * Time::GetUnScaledDeltaTime();
	}
	else // Ÿ���� �������� �ʴ°�?
	{
		mFSM = eMonsterStateType::WALK;
	}
}

void MonsterScript::UpdateHit()
{

}

void MonsterScript::UpdateGroggy()
{
	if (mType.val == (int)eMonsterType::A)
	{
		m_pHeadCollider->SetPosition(Vector3{ 0,30,0 });
		m_pBodyCollider->SetPosition(Vector3{ 0,3,0 });
		m_pBodyCollider->SetExtents(Vector3{ 28,22,28 });
	}
	else
	{
		m_pHeadCollider->SetPosition(Vector3{ 0,2,0 });
		m_pBodyCollider->SetPosition(Vector3{ 0,0,40 });
		m_pBodyCollider->SetExtents(Vector3{ 35,6,20 });

		mFSM = eMonsterStateType::DEAD;

		m_pAudioSource->Play(MONSTER_SFX_DEAD);
		return;
	}

	if (mGroggyCount < mGroggyTick.val)
	{
		// GroggyCount ����
		mGroggyCount += Time::GetUnScaledDeltaTime();

		// N�� �ȿ� ���� �پ��ٸ�
		if (m_pWeakness)
		{
  			if (m_pWeakness->GetComponent<BurnObjectScript>()->IsBurning())
			{
				mFSM = eMonsterStateType::DEAD;
				m_pAudioSource->Play(MONSTER_SFX_DEAD);
				mGroggyCount = 0.f;
			}
		}
	}
	else
	{
		if (mType.val == (int)eMonsterType::A)
		{
			m_pHeadCollider->SetPosition(Vector3{ 0,40,0 });
			m_pBodyCollider->SetPosition(Vector3{ 0,20,0 });
			m_pBodyCollider->SetExtents(Vector3{ 28,40,28 });
		}

		mGroggyCount = 0.f;
		mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::UpdateDead()
{
	if (m_pAnimator)
	{
		if (mType.val == (int)eMonsterType::B)
		{
			mResetCount += Time::GetUnScaledDeltaTime();

			if (mResetCount > 3.f)
			{
				mResetCount = 0.f;
				gameObject->transform->GetParent()->gameObject->SetActive(false);
				return;
			}
		}

		if (m_pAnimator->IsEnd())
		{
			// �ִϸ��̼��� ������ �� �ʵ� ���� ��Ȱ��ȭ
			mResetCount += Time::GetUnScaledDeltaTime();

			if (mResetCount > 3.f)
			{
				mResetCount = 0.f;
				gameObject->transform->GetParent()->gameObject->SetActive(false);
			}
		}
	}
}

void MonsterScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	// �����̶� �÷��̾ �ε����� ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player)
		{
			if (mFSM != eMonsterStateType::DEAD && mFSM != eMonsterStateType::GROGGY)
			{
				//mFSM = eMonsterStateType::HIT;
				// 3��ŭ ������ ������
				player->Hit(mDamage.val);
			}
		}

		// ������ ������ �ٽ� ó�� ���·�
		//mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
	// �����̶� �÷��̾ �ε����� ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		auto* player = _destination->GetOwner()->GetComponent<PlayerScript>();
		if (player)
		{
			if (mFSM != eMonsterStateType::DEAD && mFSM != eMonsterStateType::GROGGY)
			{
				// 3��ŭ ������ ������
				player->Hit(mDamage.val);
			}
		}

		// ������ ������ �ٽ� ó�� ���·�
		//mFSM = eMonsterStateType::IDLE;
	}
}

void MonsterScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{

}

void MonsterScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	// �Ӹ� ������ ��
	if (_destination->gameObject->GetTag() == L"Player")
	{
		if (m_pTarget)
		{
			auto* player = m_pTarget->GetComponent<PlayerScript>();
			if (player)
			{
				player->Jump(0.5f, false);
			}
		}

		// GROGGY�� �ٲ��ֱ�
		mFSM = eMonsterStateType::GROGGY;
		m_pAudioSource->Play(MONSTER_SFX_STEP);
	}
}

void MonsterScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
}

void MonsterScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void MonsterScript::SetSFX(const std::wstring& _filename)
{
	ResourceHandle ButtonSoundHandle;
	ButtonSoundHandle.mResourceType = eResourceType::AudioResource;
	ButtonSoundHandle.mMainKey = _filename;
	ButtonSoundHandle.mPath = L"resource/sound/" + _filename;
	if (ResourceManager::GetResource<AudioResource>(ButtonSoundHandle) == nullptr)
	{
		ResourceManager::LoadFileFromHandle(ButtonSoundHandle);
	}
	m_pAudioSource->AddAudio(_filename, ButtonSoundHandle);
}

void MonsterScript::InitSFX()
{
	m_pAudioSource = gameObject->AddComponent<AudioSource>();
	SetSFX(MONSTER_SFX_STEP);
	SetSFX(MONSTER_SFX_DEAD);
}

void MonsterScript::UpdateMonsterAnim_A()
{
	if (!m_pAnimator)
		return;

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE_A, 0.5f);
		break;
	case eMonsterStateType::WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_WALK_A, 0.5f);
		break;
	case eMonsterStateType::FAST_WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_FAST_WALK_A, 0.5f);
		break;
	case eMonsterStateType::RUN:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_RUN_A, 0.5f);
		break;
	case eMonsterStateType::HIT:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_HIT_A, 0.5f);
		break;
	case eMonsterStateType::GROGGY:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_GROGGY_A, 0.5f);
		break;
	case eMonsterStateType::DEAD:
		break;
	}
}

void MonsterScript::UpdateMonsterAnim_B()
{
	if (!m_pAnimator)
		return;

	switch (mFSM)
	{
	case eMonsterStateType::IDLE:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE_B, 0.5f);
		break;
	case eMonsterStateType::WALK:
		m_pAnimator->SetLoop(true);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_IDLE_B, 0.5f);
		break;
	case eMonsterStateType::ROTATE:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_ROTATE_B, 0.5f);
		break;
	case eMonsterStateType::HIT:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_HIT_B, 0.5f);
		break;
	case eMonsterStateType::GROGGY:
		m_pAnimator->SetLoop(false);
		m_pAnimator->SetCurrentAnimation(MONSTER_ANIM_GROGGY_B, 0.5f);
		break;
	case eMonsterStateType::DEAD:
		break;
	}
}

void MonsterScript::UpdateMonsterAngle()
{

	if (mTargetDir != Vector3::Zero)
	{
		float targetAngleY = atan2(-mTargetDir.x, -mTargetDir.z);

		if (targetAngleY < 0.0f)
		{
			targetAngleY += XM_2PI;
		}

		Vector3 currAngle = gameObject->transform->GetEulerAngles();
		float currAngleY = currAngle.y;
		float delta = fmod(targetAngleY - currAngleY + XM_PI, XM_2PI) - XM_PI;
		float newAngleY = currAngleY + delta * 0.7f;

		gameObject->transform->SetEulerAngles(Vector3(0.f, newAngleY, 0.f));
	}
}