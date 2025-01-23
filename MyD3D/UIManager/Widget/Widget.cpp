#include "pch.h"
#include "UIManager/Widget/Widget.h"

#include "UIManager/UIManager.h"

/*	
	DXGI_MODE_ROTATION

	DXGI_MODE_ROTATION_UNSPECIFIED = 0,
	DXGI_MODE_ROTATION_IDENTITY = 1,
	DXGI_MODE_ROTATION_ROTATE90 = 2,
	DXGI_MODE_ROTATION_ROTATE180 = 3,
	DXGI_MODE_ROTATION_ROTATE270 = 4
*/

Widget::Widget()
{

}

Widget::~Widget()
{

}

void Widget::Init()
{
	m_pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());
}

void Widget::Update()
{

}

void Widget::Release()
{
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSpriteBatch != nullptr)
	{
		delete m_pSpriteBatch;
		m_pSpriteBatch = nullptr;
	}
}

