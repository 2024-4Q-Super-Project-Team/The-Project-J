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
		// �̹��� ũ�� ����
		gameObject->transform->scale.x = _texture->Texture->mWidth;
		gameObject->transform->scale.y = _texture->Texture->mHeight;
	}
}

void UIWidget::ProcessFadeIn(Color* _color)
{
    mFadeState = eFadeState::FADE_IN;

    if (a >= 255)
    {
        _color->w = 1.f;
        mFadeState = eFadeState::IDLE;
        return;
    }

    // ���� ����
    a += (Time::GetUnScaledDeltaTime() / mFadeTime) * 255;
    if (a > 255) a = 255; // ���� �ʰ� ����

    // ���� ���� ���� (���� �� ����)
    float linearAlpha = a / 255.0f;
    _color->w = pow(linearAlpha, 2.2f); // ���� ����

    // ���� ���� �ʹ� ������ 0���� ó������ �ʵ��� ���Ѱ� ����
    if (_color->w < 0.01f) _color->w = 0.01f;  // �ʹ� ���� �� ����
}

void UIWidget::ProcessFadeOut(Color* _color)
{
    mFadeState = eFadeState::FADE_OUT;

    if (a <= 127.0f)
    {
        _color->w = 0.f;
        mFadeState = eFadeState::IDLE;
        return;
    }

    // ���� ����
    a -= (Time::GetUnScaledDeltaTime() / mFadeTime) * 255;
    if (a < 127) a = 127; // ���� �ʰ� ����

    // ���� ���� ���� (���� �� ����)
    float linearAlpha = a / 255.0f;
    _color->w = pow(linearAlpha, 2.2f); // ���� ����

    // ���� ���� �ʹ� ������ 0���� ó������ �ʵ��� ���Ѱ� ����
    if (_color->w < 0.01f) _color->w = 0.01f;  // �ʹ� ���� �� ����
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
