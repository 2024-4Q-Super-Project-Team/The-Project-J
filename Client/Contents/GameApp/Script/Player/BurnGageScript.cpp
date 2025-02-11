#include "pch.h"
#include "BurnGageScript.h"
#include "PlayerScript.h"

void BurnGageScript::Start()
{
	auto& roodChild = gameObject->transform->GetParent()->GetChildren();
	for (Transform* child : roodChild)
	{
		if (child->gameObject->GetName() == L"Player_Body")
		{
			m_pPlayer = child->gameObject;
		}
	}

	auto& children = gameObject->transform->GetChildren();
	for (Transform* child : children)
	{
		if (child->gameObject->GetName() == L"background")
		{
			m_pBackground = child->gameObject;
			m_pBackground->SetTag(L"Sprite");
			m_pBackground->SetActive(false);
		}
		else if (child->gameObject->GetName() == L"gauge")
		{
			m_pGauge = child->gameObject;
			m_pGauge->SetTag(L"Sprite");
			m_pGauge->SetActive(false);
		}
		else if (child->gameObject->GetName() == L"gaugeText")
		{
			m_pText = child->gameObject;
			m_pText->SetTag(L"Sprite");
			m_pText->SetActive(false);
		}
	}
}

void BurnGageScript::Update()
{
	auto* player = m_pPlayer->GetComponent<PlayerScript>();

	if (player->CanMoveFire())
	{
		if (player->GetState() == ePlayerStateType::MOVE_FIRE)
		{
			mState = eGaugeState::ACTIVE;
		}
		else
		{
			mState = eGaugeState::PASSIVE;
		}
	}
	else
	{
		mState = eGaugeState::NONE;
	}

	switch (mState)
	{
	case eGaugeState::NONE:
		m_pBackground->SetActive(false);
		m_pText->SetActive(false);
		m_pGauge->SetActive(false);
		m_pGauge->transform->scale.x = 0;
		break;
	case eGaugeState::PASSIVE:
		m_pBackground->SetActive(true);
		m_pText->SetActive(true);
		//m_pGauge->SetActive(false);
		m_pGauge->transform->scale.x = 0;
		break;
	case eGaugeState::ACTIVE:
		m_pBackground->SetActive(true);
		m_pText->SetActive(true);
		m_pGauge->SetActive(true);
		ProcessGauge();
		break;
	}
}

void BurnGageScript::ProcessGauge()
{
	auto* gauge = m_pGauge->GetComponent<UISprite>();
	auto* player = m_pPlayer->GetComponent<PlayerScript>();
	
	if (gauge && player)
	{
		if (m_pGauge->transform->scale.x <= 193.0f)
		{
			m_pGauge->transform->scale.x = player->GetRatio() * 193.0f;
		}
		else
		{
			m_pGauge->transform->scale.x = std::min(m_pGauge->transform->scale.x, 193.0f);
		}
	}
}
