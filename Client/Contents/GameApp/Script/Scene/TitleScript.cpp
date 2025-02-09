#include "pch.h"
#include "TitleScript.h"

void TitleScript::Start()
{
	// Component Load
	// canvas
	gameObject->AddComponent<Canvas>();

	// object load
	// text
	Object* title = CreateObject(L"title", L"Sprite");
	title->AddComponent<UISprite>();
	title->transform->SetParent(gameObject->transform);
	SetPosition(title, { 474,320 });

	Object* startButton = CreateObject(L"startButton", L"Button");
	auto* sb = startButton->AddComponent<UIButton>();
	sb->SetState(eButtonState::SELECTED);
	startButton->transform->SetParent(gameObject->transform);
	SetPosition(startButton, { 1695,602 });

	Object* keyButton = CreateObject(L"keyButton", L"Button");
	auto* kb = keyButton->AddComponent<UIButton>();
	kb->SetState(eButtonState::DEFAULT);
	keyButton->transform->SetParent(gameObject->transform);
	SetPosition(keyButton, { 1679,714 });

	Object* creditButton = CreateObject(L"creditButton", L"Button");
	auto* cb = creditButton->AddComponent<UIButton>();
	cb->SetState(eButtonState::DEFAULT);
	creditButton->transform->SetParent(gameObject->transform);
	SetPosition(creditButton, { 1680,826 });

	Object* exitButton = CreateObject(L"exitButton", L"Button");
	auto* eb = exitButton->AddComponent<UIButton>();
	eb->SetState(eButtonState::DEFAULT);
	exitButton->transform->SetParent(gameObject->transform);
	SetPosition(exitButton, { 1694,938 });

	Object* control = CreateObject(L"control", L"Sprite");
	control->AddComponent<UISprite>();
	control->transform->SetParent(gameObject->transform);
	SetPosition(control, { 0,0 });
	control->SetActive(false);
}

void TitleScript::Update()
{
	if (InputSyncer::GetInputDirection(0).y > 0)
	{
		if (mSelectPos > 0)
		{
			mSelectPos--;
		}
	}
	else if (InputSyncer::GetInputDirection(0).y < 0)
	{
		if (mSelectPos < 3)
		{
			mSelectPos++;
		}
	}

	switch (mSelectPos)
	{
	case 0:
		// 스타트버튼 select 모드
		// 나머진 디폴트
		StartButtonUpdate();
		break;
	case 1:
		// 조작키 버튼 select 모드
		// 나머진 디폴트
		KeyButtonUpdate();
		break;
	case 2:
		// 크레딧 버튼 select 모드
		// 나머진 디폴트
		CreditButtonUpdate();
		break;
	case 3:
		// 종료 버튼 select 모드
		// 나머진 디폴트
		ExitButtonUpdate();
		break;
	}
}

void TitleScript::StartButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// 누를 시 월드 이동
	}
}

void TitleScript::KeyButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// 누를 시 조작키 창 띄우기
	}
}

void TitleScript::CreditButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// 누를 시 크레딧 월드로 이동
	}
}

void TitleScript::ExitButtonUpdate()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// 누를 시 게임 종료
	}
}

void TitleScript::SetPosition(Object* _obj, Vector2 _pos)
{
	_obj->transform->position = Vector3(_pos.x, _pos.y, 0);
}
