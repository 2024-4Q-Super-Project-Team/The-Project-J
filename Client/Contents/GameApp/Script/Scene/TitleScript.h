#pragma once
#include "Helper/InputSyncer.h"

class TitleScript
	: public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(TitleScript);
public:
	void Start();
	void Update();
public:
	void StartButtonUpdate();
	void KeyButtonUpdate();
	void CreditButtonUpdate();
	void ExitButtonUpdate();
private:
	UINT mSelectPos = 0;
	bool bIsMove = false;
	bool bIsControl = false;

	Object* title = nullptr;
	Object* startButton = nullptr;
	Object* keyButton = nullptr;
	Object* creditButton = nullptr;
	Object* exitButton = nullptr;
	Object* control = nullptr;
};

