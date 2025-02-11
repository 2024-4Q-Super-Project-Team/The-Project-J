#include "pch.h"
#include "OpeningScript.h"

static int index = 0;

void OpeningScript::Start()
{
	index = 0;
	
	// 책 fbx
	book = FindObject(L"Book", L"book");

	if (book)
	{
		book->transform->MoveTo(Vector3(140, 0, -1020), 2.f);
		bookAnim = book->GetComponent<Animator>();
	}

	if (bookAnim)
	{
		bookAnim->SetLoop(false);
	}

	// Component Load
	// canvas
	gameObject->AddComponent<Canvas>();

	// object load
	// Page01
	{
		fadeBox = FindObjectWithName(L"fade_box_01");
		auto* fade = fadeBox->GetComponent<UISprite>();
		fadeBox->transform->SetParent(gameObject->transform);
		fadeBox->SetActive(false);

		Object* text_01 = FindObject(L"text_01", L"Sprite");
		auto* t1 = text_01->GetComponent<UISprite>();
		text_01->transform->SetParent(gameObject->transform);
		text_01->SetActive(false);
		mWidgetList.emplace_back(t1);

		Object* text_02 = FindObject(L"text_02", L"Sprite");
		auto* t2 = text_02->GetComponent<UISprite>();
		text_02->transform->SetParent(gameObject->transform);
		text_02->SetActive(false);
		mWidgetList.emplace_back(t2);

		Object* image_01 = FindObject(L"image_01", L"Sprite");
		auto* i1 = image_01->GetComponent<UISprite>();
		image_01->transform->SetParent(gameObject->transform);
		image_01->SetActive(false);
		mWidgetList.emplace_back(i1);
	}

	// Page02
	{
		Object* text_03 = FindObject(L"text_03", L"Sprite");
		auto* t3 = text_03->GetComponent<UISprite>();
		text_03->transform->SetParent(gameObject->transform);
		text_03->SetActive(false);
		mWidgetList.emplace_back(t3);

		Object* text_04 = FindObject(L"text_04", L"Sprite");
		auto* t4 = text_04->GetComponent<UISprite>();
		text_04->transform->SetParent(gameObject->transform);
		text_04->SetActive(false);
		mWidgetList.emplace_back(t4);

		Object* text_05 = FindObject(L"text_05", L"Sprite");
		auto* t5 = text_05->GetComponent<UISprite>();
		text_05->transform->SetParent(gameObject->transform);
		text_05->SetActive(false);
		mWidgetList.emplace_back(t5);

		Object* image_02 = FindObject(L"image_02", L"Sprite");
		auto* i2 = image_02->GetComponent<UISprite>();
		image_02->transform->SetParent(gameObject->transform);
		image_02->SetActive(false);
		mWidgetList.emplace_back(i2);
	}

	// Page03
	{
		Object* text_06 = FindObject(L"text_06", L"Sprite");
		auto* t6 = text_06->GetComponent<UISprite>();
		text_06->transform->SetParent(gameObject->transform);
		text_06->SetActive(false);
		mWidgetList.emplace_back(t6);

		Object* text_07 = FindObject(L"text_07", L"Sprite");
		auto* t7 = text_07->GetComponent<UISprite>();
		text_07->transform->SetParent(gameObject->transform);
		text_07->SetActive(false);
		mWidgetList.emplace_back(t7);

		Object* text_08 = FindObject(L"text_08", L"Sprite");
		auto* t8 = text_08->GetComponent<UISprite>();
		text_08->transform->SetParent(gameObject->transform);
		text_08->SetActive(false);
		mWidgetList.emplace_back(t8);

		Object* image_03 = FindObject(L"image_03", L"Sprite");
		auto* i3 = image_03->GetComponent<UISprite>();
		image_03->transform->SetParent(gameObject->transform);
		image_03->SetActive(false);
		mWidgetList.emplace_back(i3);
	}
}

void OpeningScript::Update()
{
	if (bookAnim)
	{
		if (bookAnim->GetDuration() >= 25.0f)
		{
			bookAnim->Pause();
			bookAnim->SetFrame(25.0f);
		}
	}

	switch (mPageType)
	{
	case 0:
		break;
	case 1:
		Page01Update();
		break;
	case 2:

		Page02Update();
		break;
	case 3:
		Page03Update();
		break;
	}
}

void OpeningScript::Page01Update()
{
	if (index == 3)
	{
		if (!startMoving)
		{
			// 시간 증가
			timer += Time::GetUnScaledDeltaTime();

			// 3초 기다린 후 이동 시작
			if (timer >= 3.f)
			{
				startMoving = true;
			}
		}
		else
		{
			mWidgetList[0]->ProcessFadeOut(&mWidgetList[0]->mColor);
			mWidgetList[1]->ProcessFadeOut(&mWidgetList[1]->mColor);
			mWidgetList[2]->ProcessFadeOut(&mWidgetList[2]->mColor);
		}

		if (startMoving && mWidgetList[0]->GetFade() == eFadeState::IDLE)
		{
			mWidgetList[0]->gameObject->SetActive(false);
			mWidgetList[1]->gameObject->SetActive(false);
			mWidgetList[2]->gameObject->SetActive(false);

			mPageType = 2;
			startMoving = false;
			timer = 0.f;
		}
	}

	if (InputSyncer::IsKeyDown(0,InputSyncer::eInputType::JUMP) && index < 3)
	{
		mWidgetList[index]->gameObject->SetActive(true);
	}

	if (mWidgetList[index]->gameObject->GetState() == EntityState::Active)
	{
		mWidgetList[index]->ProcessFadeIn(&mWidgetList[index]->mColor);

		if (mWidgetList[index]->GetFade() == eFadeState::IDLE)
		{
			if(index < 3)
				index += 1;
		}
	}
}

void OpeningScript::Page02Update()
{
	if (index == 7)
	{
		if (!startMoving)
		{
			// 시간 증가
			timer += Time::GetUnScaledDeltaTime();

			// 3초 기다린 후 이동 시작
			if (timer >= 3.f)
			{
				startMoving = true;
			}
		}
		else
		{
			mWidgetList[3]->ProcessFadeOut(&mWidgetList[3]->mColor);
			mWidgetList[4]->ProcessFadeOut(&mWidgetList[4]->mColor);
			mWidgetList[5]->ProcessFadeOut(&mWidgetList[5]->mColor);
			mWidgetList[6]->ProcessFadeOut(&mWidgetList[6]->mColor);
		}

		if (startMoving && mWidgetList[6]->GetFade() == eFadeState::IDLE)
		{
			mWidgetList[3]->gameObject->SetActive(false);
			mWidgetList[4]->gameObject->SetActive(false);
			mWidgetList[5]->gameObject->SetActive(false);
			mWidgetList[6]->gameObject->SetActive(false);

			mPageType = 3;
			startMoving = false;
			timer = 0.f;
		}
	}

	if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP) && index < 7)
	{
		mWidgetList[index]->gameObject->SetActive(true);
	}

	if (mWidgetList[index]->gameObject->GetState() == EntityState::Active)
	{
		mWidgetList[index]->ProcessFadeIn(&mWidgetList[index]->mColor);

		if (mWidgetList[index]->GetFade() == eFadeState::IDLE)
		{
			if (index < 7)
				index += 1;
		}
	}
}

void OpeningScript::Page03Update()
{
	if (index == 11)
	{
		if (!startMoving)
		{
			// 시간 증가
			timer += Time::GetUnScaledDeltaTime();

			// 3초 기다린 후 이동 시작
			if (timer >= 3.f)
			{
				startMoving = true;
			}
		}
		else
		{
			fadeBox->SetActive(true);

			auto* fade = fadeBox->GetComponent<UISprite>();

			if (fadeBox->GetState() == EntityState::Active)
			{
				if (fade)
				{
					fade->ProcessFadeIn(&fade->mColor);
				}

				if (fade->GetFade() == eFadeState::IDLE)
				{
					mPageType = 0;
				}
			}
		}
	}
	else 
	{
		if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP) && index < 11)
		{
			mWidgetList[index]->gameObject->SetActive(true);
		}

		if (mWidgetList[index]->gameObject->GetState() == EntityState::Active && index < 11)
		{
			mWidgetList[index]->ProcessFadeIn(&mWidgetList[index]->mColor);

			if (mWidgetList[index]->GetFade() == eFadeState::IDLE)
			{
				if (index < 11)
					index += 1;
			}
		}
	}
}
