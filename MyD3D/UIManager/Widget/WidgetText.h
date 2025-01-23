#pragma once
#include "Widget.h"

struct TextInfo
{
	float line = 15.f;
	char defaultText = '_';
	wchar_t msg[1024];
	Rect textBox{};
};

class WidgetText
	: public Widget
{
public:
	WidgetText();
	virtual ~WidgetText(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
public:
	void SetTextLine(float _line) { mTextInfo.line = _line; }
	void SetTextDefault(char _defaultText) { mTextInfo.defaultText = _defaultText; }
	void SetTextFormat(const wchar_t* _msg, ...);
public:

	Rect			GetTextBox()	{ return mTextInfo.textBox; }
	std::wstring	GetTextFormat()	{ return mTextInfo.msg; }
private:
	void OutlinedTextRender(float _offset, Color _outlineColor);

	SpriteFont* m_pSpriteFont = nullptr;

	TextInfo mTextInfo;
};

