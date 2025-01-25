#include "pch.h"
#include "WidgetButton.h"

WidgetButton::WidgetButton()
{

}

WidgetButton::~WidgetButton()
{

}

void WidgetButton::Init()
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

	mSize.x = m_pTexture->Texture->mWidth;
	mSize.y = m_pTexture->Texture->mHeight;

	DebugRectangleInit();
}

void WidgetButton::PreUpdate()
{
	Vector2 mousePos = Input::GetCurrentMousePos();
	
	/*if (mPosition.x * _scale.x < mousePos.x &&
		mPosition.y * _scale.y < mousePos.y &&
		mPosition.x * _scale.x + mousePos.x > mousePos.x &&
		mPosition.y * _scale.y + mousePos.y > mousePos.y)
	{
		bIsEnter = true;
	}
	else
	{
		bIsEnter = false;
	}*/
}

void WidgetButton::Update()
{
	Widget::Update();

	if (IsEnter())
	{
		if (OnClick())
			Display::Console::Log("OnClick", '\n');

		//if (OnHold())
			//Display::Console::Log("OnHold", '\n');

		if (OnRelease())
			Display::Console::Log("OnRelease", '\n');
	}
	else
	{
		//if (OnExit())
			//Display::Console::Log("OnExit", '\n');
	}
}

void WidgetButton::Render(Vector2 _scale)
{
	if (IsEnter())
	{
		DebugRectangleRender(Color(1, 0, 0), _scale);
	}
	else
	{
		DebugRectangleRender(Color(1, 1, 1), _scale);
	}

	RECT rect{};
	rect.left = mPosition.x * _scale.x;
	rect.top = mPosition.y * _scale.y;
	rect.right = rect.left + (mSize.x * _scale.x);
	rect.bottom = rect.top + (mSize.y * _scale.y);

	UIManager::pSpriteBatch->Draw(m_pTexture->Texture->mSRV, rect, nullptr);
}

void WidgetButton::Release()
{
	Widget::Release();
}
