#pragma once
#include "Widget.h"

class WidgetImage
	: public Widget
{
public:
	WidgetImage();
	virtual ~WidgetImage(); // ������ �����ߵǴµ� �� �ȵ�?

public:
	virtual void Start();
	virtual void Update();
	virtual void Render();
public:

private:
	SpriteBatch* m_pSpriteBatch = nullptr;
};

