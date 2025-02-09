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
	void SetPosition(Object* _obj, Vector2 _pos);
private:
	UINT mSelectPos = 0;
};

