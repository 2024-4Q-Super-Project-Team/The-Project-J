#include "pch.h"
#include "OpeningScript.h"

void OpeningScript::Start()
{
	// 책 fbx 로드
	// Object* book = CreateObject(L"book", L"Book");
	// book->AddComponent<Animator>();

	// Component Load
	// canvas
	gameObject->AddComponent<Canvas>();

	// object load
	// text
	Object* text_01 = CreateObject(L"text_01", L"Sprite");
	text_01->AddComponent<UISprite>();
	text_01->transform->SetParent(gameObject->transform);
	SetPosition(text_01, { 474,320 });
	text_01->SetActive(false);

	Object* text_02 = CreateObject(L"text_02", L"Sprite");
	text_02->AddComponent<UISprite>();
	text_02->transform->SetParent(gameObject->transform);
	SetPosition(text_02, { 276,387 });
	text_02->SetActive(false);

	Object* text_03 = CreateObject(L"text_03", L"Sprite");
	text_03->AddComponent<UISprite>();
	text_03->transform->SetParent(gameObject->transform);
	SetPosition(text_03, { 392,643 });
	text_03->SetActive(false);

	Object* text_04 = CreateObject(L"text_04", L"Sprite");
	text_04->AddComponent<UISprite>();
	text_04->transform->SetParent(gameObject->transform);
	SetPosition(text_04, { 333,710 });
	text_04->SetActive(false);
	
	// image
	Object* image_01 = CreateObject(L"image_01", L"Sprite");
	image_01->AddComponent<UISprite>();
	image_01->transform->SetParent(gameObject->transform);
	SetPosition(image_01, { 989,147 });
	image_01->SetActive(false);
}

void OpeningScript::Update()
{
	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP))
	{
		// 누를 시 하나씩 Set Active true
	}
}

void OpeningScript::SetPosition(Object* _obj, Vector2 _pos)
{
	_obj->transform->position = Vector3(_pos.x, _pos.y, 0);
}
