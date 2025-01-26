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

Widget::Widget(Object* _owner)
	: Component(_owner)
{

}

Widget::~Widget()
{
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Widget::SetTexture(ResourceHandle _handle)
{
	m_pTexture = ResourceManager::GetResource<Texture2DResource>(_handle);

	// 이미지 크기 설정
	gameObject->transform->scale.x = m_pTexture->Texture->mWidth;
	gameObject->transform->scale.y = m_pTexture->Texture->mHeight;
}