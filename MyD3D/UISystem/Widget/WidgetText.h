#pragma once
#include "Widget.h"

struct TextInfo
{
	float line = 15.f;
	char defaultText = '_';
	Color color{};
	wchar_t msg[1024];
};

class WidgetText
	: public Widget
{
public:
	WidgetText();
	virtual ~WidgetText(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Start();
	virtual void Update();
	virtual void Render();
public:
	void SetTextLine(float _line) { mTextInfo.line = _line; }
	void SetTextDefault(char _defaultText) { mTextInfo.defaultText = _defaultText; }
	void SetTextColor(Color _color) { mTextInfo.color = _color; }
	void SetTextFormat(const wchar_t* _msg, ...);
private:
	SpriteFont* m_pSpriteFont = nullptr;
	SpriteBatch* m_pSpriteBatch = nullptr;

	TextInfo mTextInfo;
};

