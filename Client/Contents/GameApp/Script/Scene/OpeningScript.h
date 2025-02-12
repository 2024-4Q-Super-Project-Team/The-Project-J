#pragma once
#include "Helper/InputSyncer.h"

class OpeningScript
	: public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(OpeningScript);
public:
	void Start();
	void Update();
	void Page01Update();
	void Page02Update();
	void Page03Update();

private:
	std::vector<UISprite*> mWidgetList;
	Object* curObject = nullptr;
	Object* book = nullptr;
	Object* fadeBox = nullptr;

	Animator* bookAnim = nullptr;
	AudioSource* m_pAudio = nullptr;

	World* mainWorld = nullptr;

	float timer = 0.f;
	bool startMoving = false;
	UINT mPageType = 1;	// 현재 페이지 타입
};
