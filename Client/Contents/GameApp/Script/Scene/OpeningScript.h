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
public:
	void SetPosition(Object* _obj, Vector2 _pos);
};
