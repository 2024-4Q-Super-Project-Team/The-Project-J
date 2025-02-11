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
public:

	Object* curObject = nullptr;
	std::vector<UISprite*> mWidgetList;

	Animator* bookAnim = nullptr;
	Animator* paperAnim = nullptr;
	Object* book = nullptr;
	Object* paper = nullptr;
	UINT mPageType = 1;	// 현재 페이지 타입
};
