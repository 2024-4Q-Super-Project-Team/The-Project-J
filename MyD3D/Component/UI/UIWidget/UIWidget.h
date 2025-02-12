#pragma once
#include "Component/Component.h"

class Transform;
class Texture2DResource;

// 위젯 타입
enum class eUIType
{
	NON,	// 없음
	IMAGE,	// 이미지
	TEXT,	// 텍스트
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
	// 기본 정보 설정
	void SetTexture(ResourceHandle _handle, Texture2DResource*& _texture);
	void ProcessFadeIn();
	void ProcessFadeOut();
	void SetFadeState(eFadeState _fade) { mFadeState = _fade; }
	void SetAlpha(float _alpha) { alpha = _alpha; }
	float GetAlpha()	 { return alpha; }
	eFadeState GetFade() { return mFadeState; }

	// 그래픽 정보 설정
	void	SetColor(Color _color)	{ mColor = _color; }
	Color	GetColor()				{ return mColor; }
	RECT rect{};

protected:
	std::wstring mID = L"";

	float alpha = 0.f;
	Color	mColor{ 1,1,1,1 };
	eUIType	mUIType = eUIType::NON;
	float mFadeTime = 1.0f;
	eFadeState mFadeState = eFadeState::IDLE;
};