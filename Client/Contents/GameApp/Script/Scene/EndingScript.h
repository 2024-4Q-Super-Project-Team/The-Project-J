#pragma once
#include "Helper/InputSyncer.h"

class EndingScript
	: public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(EndingScript);
public:
	void Start();
	void Update();
	void PageUpdate();
	void CreditUpdate();
	void CreditPosUpdate();
public:
	std::vector<UISprite*> mWidgetList;

	Animator* bookAnim = nullptr;
	Object* book = nullptr;
	Object* credit = nullptr;
	Object* fadeBox = nullptr;
	UINT mPageType = 1;	// 현재 페이지 타입

	AudioSource* m_pAudio = nullptr;

	bool startMoving = false;  // 이동 시작 여부
	float timer = 0;

	SerializeField(FLOAT, waitTime, 3.f);
	SerializeField(FLOAT, moveSpeed, 100.f);
};