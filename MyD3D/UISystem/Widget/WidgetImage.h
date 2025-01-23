#pragma once
#include "Widget.h"

class WidgetImage
	: public Widget
{
public:
	WidgetImage();
	virtual ~WidgetImage(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Start();
	virtual void Update();
	virtual void Render();
public:

private:
	SpriteBatch* m_pSpriteBatch = nullptr;
};

