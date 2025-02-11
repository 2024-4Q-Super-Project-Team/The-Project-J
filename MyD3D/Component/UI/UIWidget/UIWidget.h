#pragma once
#include "Component/Component.h"

class Transform;
class Texture2DResource;

// ���� Ÿ��
enum class eUIType
{
	NON,	// ����
	IMAGE,	// �̹���
	TEXT,	// �ؽ�Ʈ
};

enum class eFadeState
{
	IDLE,
	FADE_IN,
	FADE_OUT,
};

class IWidgetContext
{
public:
	virtual void DrawWidget(Vector2 _scale) = 0;
};

class Camera;

class UIWidget
	: public Component
	, public IWidgetContext
{
public:
	explicit UIWidget(Object* _owner);
	virtual ~UIWidget();
public:
	virtual void Start() override;
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Draw(Camera* _camera) override;
	virtual void PostRender() override;
	// Editor Only
	virtual void EditorUpdate() override;
	virtual void EditorRender() override;
	virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
	// �⺻ ���� ����
	void SetTexture(ResourceHandle _handle, Texture2DResource*& _texture);
	void ProcessFadeIn(Color* _color);
	void ProcessFadeOut(Color* _color);
	void SetFade(eFadeState _fade) { mFadeState = _fade; }
	eFadeState GetFade() { return mFadeState; }

	// �׷��� ���� ����
	void	SetColor(Color _color)	{ mColor = _color; }
	Color	GetColor()				{ return mColor; }
	RECT rect{};
	Color	mColor{ 1,1,1,0 };
	float a = 127.f;

protected:
	std::wstring mID = L"";

	eUIType	mUIType = eUIType::NON;
	float mFadeTime = 5.f;
	eFadeState mFadeState = eFadeState::IDLE;
};