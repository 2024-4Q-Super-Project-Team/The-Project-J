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

	// �ؽ�Ʈ �ڽ� ũ�⸦ ��� �Լ�
	RECT rect = m_pSpriteFont->MeasureDrawBounds(mTextInfo.msg, mPosition);
	mTextInfo.textBox = Rect(rect.left, rect.top, rect.right, rect.bottom);
}

void WidgetText::Render()
{
	m_pSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr);

	if (bUseOutline)
	{
		OutlinedTextRender(mOutlineInfo.scale, mOutlineInfo.color);
	}

	m_pSpriteFont->DrawString(m_pSpriteBatch, mTextInfo.msg, mPosition, mColor);
	m_pSpriteBatch->End();
}

void WidgetText::OutlinedTextRender(float _offset, Color _outlineColor) {
	
	// �ܰ��� ������ (4����)
	XMFLOAT2 offsets[] = {
		{-_offset, 0.0f}, {_offset, 0.0f}, {0.0f, -_offset}, {0.0f, _offset}
	};

	// �ܰ��� �׸���
	for (auto& offset : offsets) {
		XMFLOAT2 outlinePos = { mPosition.x + offset.x, mPosition.y + offset.y };
		m_pSpriteFont->DrawString(m_pSpriteBatch, mTextInfo.msg, outlinePos, _outlineColor);
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
