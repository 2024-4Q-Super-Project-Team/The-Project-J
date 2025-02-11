#include "pch.h"
#include "Component/UI/UIWidget/UIWidget.h"

#include "Component/Camera/Camera.h"
#include "Component/UI/Canvas.h"
#include "Manager/UIManager.h"
#include "Resource/Resource.h"

/*
	DXGI_MODE_ROTATION

	DXGI_MODE_ROTATION_UNSPECIFIED = 0,
	DXGI_MODE_ROTATION_IDENTITY = 1,
	DXGI_MODE_ROTATION_ROTATE90 = 2,
	DXGI_MODE_ROTATION_ROTATE180 = 3,
	DXGI_MODE_ROTATION_ROTATE270 = 4
*/

UIWidget::UIWidget(Object* _owner)
	: Component(_owner)
{

}

UIWidget::~UIWidget()
{

}

void UIWidget::Start()
{
}

void UIWidget::Tick()
{
}

void UIWidget::FixedUpdate()
{
}

void UIWidget::PreUpdate()
{
	if (!gameObject->transform->GetParent())
		return;

	Canvas* canvas = 
		gameObject->transform->GetParent()->gameObject->GetComponent<Canvas>();

	if (canvas)
	{
		canvas->mWidgets.push_back(this);
	}
}

void UIWidget::Update()
{

}

void UIWidget::PostUpdate()
{
}

void UIWidget::PreRender()
{
}

void UIWidget::Render()
{
}

void UIWidget::Draw(Camera* _camera)
{
}

void UIWidget::PostRender()
{
}

void UIWidget::SetTexture(ResourceHandle _handle, Texture2DResource*& _texture)
{
	if (_handle.mResourceType != eResourceType::Texture2DResource)
		return;


    _texture = ResourceManager::GetResource<Texture2DResource>(_handle);

	if (_texture)
	{
		// 이미지 크기 설정
		gameObject->transform->scale.x = _texture->Texture->mWidth;
		gameObject->transform->scale.y = _texture->Texture->mHeight;
	}
}

void UIWidget::ProcessFadeIn()
{
    if (alpha >= 1.f)
    {
        alpha = 1.f;
        mFadeState = eFadeState::IDLE;
        return;
    }

    mFadeState = eFadeState::FADE_IN;
    alpha += Time::GetUnScaledDeltaTime() / mFadeTime;
}

void UIWidget::ProcessFadeOut()
{
    if (alpha <= 0.f)
    {
		alpha = 0.f;
        mFadeState = eFadeState::IDLE;
        return;
    }

    mFadeState = eFadeState::FADE_OUT;
	alpha -= Time::GetUnScaledDeltaTime() / mFadeTime;
}

void UIWidget::EditorUpdate()
{
}

void UIWidget::EditorRender()
{

}

void UIWidget::EditorRendering(EditorViewerType _viewerType)
{

}
