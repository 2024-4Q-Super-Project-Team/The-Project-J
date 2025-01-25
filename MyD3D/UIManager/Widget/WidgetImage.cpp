#include "pch.h"
#include "WidgetImage.h"

WidgetImage::WidgetImage()
{

}

WidgetImage::~WidgetImage()
{
	
}

void WidgetImage::Init()
{
	Widget::Init();

	ResourceHandle handle;
	handle.mResourceType = eResourceType::Texture2DResource;
	handle.mMainKey = mID;

	if (mFilepath.empty())
		return;

	handle.mPath = mFilepath;
	ResourceManager::RegisterResourceHandle(handle);
	m_pTexture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);

	// 이미지 크기 얻기
	mSize.x = m_pTexture->Texture->mWidth;
	mSize.y = m_pTexture->Texture->mHeight;

	//DebugRectangleoInit();
}

void WidgetImage::Update()
{
	//UIManager::pSpriteBatch->SetRotation(DXGI_MODE_ROTATION_IDENTITY);
}

void WidgetImage::Render(Vector2 _scale)
{
	RECT rect{};
	rect.left = mPosition.x * _scale.x;
	rect.top = mPosition.y * _scale.y;
	rect.right = rect.left + (mSize.x * _scale.x);
	rect.bottom = rect.top + (mSize.y * _scale.y);

	//DebugRectangleRender(Color(1,0,0));
	UIManager::pSpriteBatch->Draw(m_pTexture->Texture->mSRV, rect, nullptr, mColor);
}

void WidgetImage::Release()
{
	Widget::Release();
}
