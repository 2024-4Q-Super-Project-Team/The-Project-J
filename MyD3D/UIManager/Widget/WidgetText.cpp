#include "pch.h"
#include "WidgetText.h"

WidgetText::WidgetText()
{

}

WidgetText::~WidgetText()
{

}

void WidgetText::Init()
{
	Widget::Init();

	memset(mTextInfo.msg, 0, sizeof(mTextInfo.msg));

	m_pSpriteFont = new SpriteFont(D3DGraphicsDevice::GetDevice(), mFilepath.c_str());
}

void WidgetText::Update()
{
	m_pSpriteFont->SetLineSpacing(mTextInfo.line);
	m_pSpriteFont->SetDefaultCharacter(mTextInfo.defaultText);
}

void WidgetText::Render(Vector2 _scale)
{
	if (bUseOutline)
	{
		OutlinedTextRender(_scale);
	}

	// �ؽ�Ʈ �ڽ� ũ�⸦ ��� �Լ�
	RECT rect = m_pSpriteFont->MeasureDrawBounds(mTextInfo.msg, mPosition * _scale);
	mTextInfo.textBox = Rect(rect.left, rect.top, rect.right, rect.bottom);


	m_pSpriteFont->DrawString(UIManager::pSpriteBatch, mTextInfo.msg, mPosition * _scale, mColor, mRotate, Vector2(0.0f,0.0f), _scale.x);
}

void WidgetText::OutlinedTextRender(Vector2 _scale) {
	
	// �ܰ��� ������ (4����)
	Vector2 offsets[] = {
		{-mOutlineInfo.offset, 0.0f}, {mOutlineInfo.offset, 0.0f}, {0.0f, -mOutlineInfo.offset}, {0.0f, mOutlineInfo.offset}
	};

	// �ܰ��� �׸���
	for (auto& offset : offsets) {
		Vector2 outlinePos = Vector2(mPosition.x + offset.x, mPosition.y + offset.y) * _scale;
		m_pSpriteFont->DrawString(UIManager::pSpriteBatch, mTextInfo.msg, outlinePos, mOutlineInfo.color, mRotate, Vector2(0.0f, 0.0f), _scale.x);
	}
}

void WidgetText::Release()
{
	if (m_pSpriteFont != nullptr)
	{
		delete m_pSpriteFont;
		m_pSpriteFont = nullptr;
	}

	Widget::Release();
}

void WidgetText::SetTextFormat(const wchar_t* _msg, ...)
{
	// ���� ���� ó��
	va_list args;
	va_start(args, _msg);
	vswprintf_s(mTextInfo.msg, sizeof(mTextInfo.msg) / sizeof(wchar_t), _msg, args);
	va_end(args);
}
