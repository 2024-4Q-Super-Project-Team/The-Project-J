#include "pch.h"
#include "TitleScript.h"

void TitleScript::Start()
{
	// Component Load
	// canvas
	gameObject->AddComponent<Canvas>();

	// object load
	// text
	title = FindObject(L"title", L"Sprite");

	startButton = FindObject(L"startButton", L"Button");
	auto* sb = startButton->GetComponent<UIButton>();
	if (sb)
		sb->SetState(eButtonState::SELECTED);

	keyButton = FindObject(L"keyButton", L"Button");
	auto* kb = keyButton->GetComponent<UIButton>();
	if (kb)
		kb->SetState(eButtonState::DEFAULT);

	creditButton = FindObject(L"creditButton", L"Button");
	auto* cb = creditButton->GetComponent<UIButton>();
	if (cb)
		cb->SetState(eButtonState::DEFAULT);

	exitButton = FindObject(L"exitButton", L"Button");
	auto* eb = exitButton->GetComponent<UIButton>();
	if(eb)
		eb->SetState(eButtonState::DEFAULT);

	control = FindObject(L"control", L"Sprite");
	control->SetActive(false);
}

void TitleScript::Update()
{
	if (InputSyncer::GetInputDirection(0).y > 0 || InputSyncer::IsKeyDown(0, InputSyncer::eInputType::UP))
	{
		if (!bIsMove)
		{
			if (mSelectPos > 0)
			{
				mSelectPos--;
			}

			bIsMove = true;
		}
	}
	else if (InputSyncer::GetInputDirection(0).y < 0 || InputSyncer::IsKeyDown(0, InputSyncer::eInputType::DOWN))
	{
		if (!bIsMove)
		{
			if (mSelectPos < 3)
			{
				mSelectPos++;
			}

			bIsMove = true;
		}
	}
	else if (InputSyncer::GetInputDirection(0).y == 0)
	{
		if (!bIsControl)
		{
			bIsMove = false;
		}
	}

	switch (mSelectPos)
	{
	case 0:
		// ��ŸƮ��ư select ���
		// ������ ����Ʈ
		startButton->GetComponent<UIButton>()->SetState(eButtonState::SELECTED);
		keyButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		creditButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		exitButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);

		StartButtonUpdate();
		break;
	case 1:
		// ����Ű ��ư select ���
		// ������ ����Ʈ
		startButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		keyButton->GetComponent<UIButton>()->SetState(eButtonState::SELECTED);
		creditButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		exitButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);

		KeyButtonUpdate();
		break;
	case 2:
		// ũ���� ��ư select ���
		// ������ ����Ʈ
		startButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		keyButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		creditButton->GetComponent<UIButton>()->SetState(eButtonState::SELECTED);
		exitButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);

		CreditButtonUpdate();
		break;
	case 3:
		// ���� ��ư select ���
		// ������ ����Ʈ
		startButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		keyButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		creditButton->GetComponent<UIButton>()->SetState(eButtonState::DEFAULT);
		exitButton->GetComponent<UIButton>()->SetState(eButtonState::SELECTED);

		ExitButtonUpdate();
		break;
	}
}

void TitleScript::StartButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// ���� �� ���� �̵�
	}
}

void TitleScript::KeyButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		bIsControl = true;
		bIsMove = true;
	}

	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::OFF_FIRE))
	{
		bIsControl = false;
		bIsMove = false;
	}

	if (bIsControl)
	{
		control->SetActive(true);
	}
	else
	{
		control->SetActive(false);
	}
}

void TitleScript::CreditButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// ���� �� ũ���� ����� �̵�
	}
}

void TitleScript::ExitButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// ���� �� ���� ����
	}
}
