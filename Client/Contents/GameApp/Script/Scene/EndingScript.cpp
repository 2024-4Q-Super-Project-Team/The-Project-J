#include "pch.h"
#include "EndingScript.h"

static int index = 0;

void EndingScript::Start()
{
	index = 0;

	// 책 fbx
	book = FindObjectWithName(L"Book");

	if (book)
	{
		bookAnim = book->GetComponent<Animator>();

		if (bookAnim)
		{
			bookAnim->SetFrame(25.0f);
			bookAnim->SetLoop(false);
			bookAnim->Pause();
		}
	}

	// Component Load
	// canvas
	gameObject->AddComponent<Canvas>();

	// object load
	// Page01
	{
		fadeBox = FindObjectWithName(L"fade_box_01");
		auto* fade = fadeBox->GetComponent<UISprite>();
		if (fade)
		{
			fade->mColor.w = 1.0f;
			fade->a = 255.f;
		}
		fadeBox->transform->SetParent(gameObject->transform);
		fadeBox->SetActive(true);

		Object* image_01 = FindObjectWithName(L"c_image_01");
		auto* i1 = image_01->GetComponent<UISprite>();
		image_01->transform->SetParent(gameObject->transform);
		image_01->SetActive(false);
		mWidgetList.emplace_back(i1);

		Object* text_01 = FindObjectWithName(L"c_text_01");
		auto* t1 = text_01->GetComponent<UISprite>();
		text_01->transform->SetParent(gameObject->transform);
		text_01->SetActive(false);
		mWidgetList.emplace_back(t1);

		Object* text_02 = FindObjectWithName(L"c_text_02");
		auto* t2 = text_02->GetComponent<UISprite>();
		text_02->transform->SetParent(gameObject->transform);
		text_02->SetActive(false);
		mWidgetList.emplace_back(t2);
	}

	// credit
	{
		credit = FindObjectWithName(L"c_image_02");
		credit->transform->SetParent(gameObject->transform);
		credit->SetActive(false);
	}
}

void EndingScript::Update()
{
	auto* fade = fadeBox->GetComponent<UISprite>();

	if (fadeBox->GetState() == EntityState::Active)
	{
		bookAnim->SetFrame(25.0f);

		if (fade)
		{
			fade->ProcessFadeOut(&fade->mColor);
	
			if (fade->GetFade() == eFadeState::IDLE)
			{
	
				fadeBox->SetActive(false);
			}
		}
	}
	else if (fadeBox->GetState() == EntityState::Passive)
	{
		switch (mPageType)
		{
		case 0:
			if (bookAnim)
			{
				bookAnim->Resume();
				book->transform->MoveTo(Vector3(-260, 0, -1500), 2.f);

				if (bookAnim->IsEnd())
				{
					timer += Time::GetUnScaledDeltaTime();

					// 3초 기다린 후 이동 시작
					if (timer >= 3.f)
					{
						book->SetActive(false);
						credit->SetActive(true);
						timer = 0;
						mPageType = 2;
					}
				}
			}
			break;
		case 1:
			bookAnim->SetFrame(25.0f);
			PageUpdate();
			break;
		case 2:
			CreditUpdate();
		}
	}
}

void EndingScript::PageUpdate()
{
	if (index == 3)
	{
		mWidgetList[0]->ProcessFadeOut(&mWidgetList[0]->mColor);
		mWidgetList[1]->ProcessFadeOut(&mWidgetList[1]->mColor);
		mWidgetList[2]->ProcessFadeOut(&mWidgetList[2]->mColor);

		if (mWidgetList[0]->GetFade() == eFadeState::IDLE)
		{
			mWidgetList[0]->gameObject->SetActive(false);
			mWidgetList[1]->gameObject->SetActive(false);
			mWidgetList[2]->gameObject->SetActive(false);

			mPageType = 0;
		}
	}
	else
	{
		if (InputSyncer::IsKeyDown(0, InputSyncer::eInputType::JUMP) && index < 3)
		{
			mWidgetList[index]->gameObject->SetActive(true);
		}

		if (mWidgetList[index]->gameObject->GetState() == EntityState::Active)
		{
			mWidgetList[index]->ProcessFadeIn(&mWidgetList[index]->mColor);

			if (mWidgetList[index]->GetFade() == eFadeState::IDLE)
			{
				if (index < 3)
					index += 1;
			}
		}
	}
}

void EndingScript::CreditUpdate()
{
	//fadeBox->SetActive(false);

	if (credit->GetState() == EntityState::Active)
	{
		auto* c = credit->GetComponent<UISprite>();
		
		if (c)
		{
			c->ProcessFadeIn(&c->mColor);

			if (c->GetFade() == eFadeState::IDLE)
			{
				CreditPosUpdate();
			}
		}
	}
}

void EndingScript::CreditPosUpdate()
{
	// 시간 증가
	timer += Time::GetUnScaledDeltaTime();

	// 3초 기다린 후 이동 시작
	if (timer >= waitTime.val)
	{
		startMoving = true;
	}

	if (startMoving)
	{
		// 특정 위치까지 올라가면 종료
		if (credit->transform->position.y <= -2400.0f)
		{
			credit->GetComponent<UISprite>()->ProcessFadeOut(&credit->GetComponent<UISprite>()->mColor);

			if (credit->GetComponent<UISprite>()->GetFade() == eFadeState::IDLE)
			{
				credit->SetActive(false); // 크레딧 숨기기
			}
		}
		else
		{
			credit->transform->position.y -= Time::GetUnScaledDeltaTime() * moveSpeed.val;
		}
	}
}
