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
	
}

void Widget::PreUpdate()
{

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

	if (m_pBox != nullptr)
	{
		delete m_pBox;
		m_pBox = nullptr;
	}
}

void Widget::DebugRectangleInit()
{
	ResourceHandle handle;

	// 디버그 박스 그리기
	
	handle.mResourceType = eResourceType::Texture2DResource;
	handle.mMainKey = L"box";
	handle.mPath = L"resource/texture/debugBox.png";
	ResourceManager::RegisterResourceHandle(handle);
	m_pBox = ResourceManager::Alloc_Resource<Texture2DResource>(handle);
}

void Widget::DebugRectangleRender(Color _color, Vector2 _scale)
{
	UIManager::pSpriteBatch->Draw(m_pBox->Texture->mSRV, mPosition * _scale, nullptr, _color, 0.0f, {0, 0}, mSize * _scale);
}