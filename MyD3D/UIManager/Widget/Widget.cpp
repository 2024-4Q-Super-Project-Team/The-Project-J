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

void Widget::SetTexture(std::wstring _id, std::wstring _path)
{
	ResourceHandle handle;
	handle.mResourceType = eResourceType::Texture2DResource;
	handle.mMainKey = _id;

	if (_path.empty())
		return;

	handle.mPath = _path;
	ResourceManager::RegisterResourceHandle(handle);
	m_pTexture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);

	mID = _id;

	// 이미지 크기 설정
	gameObject->transform->scale.x = m_pTexture->Texture->mWidth;
	gameObject->transform->scale.y = m_pTexture->Texture->mHeight;
}