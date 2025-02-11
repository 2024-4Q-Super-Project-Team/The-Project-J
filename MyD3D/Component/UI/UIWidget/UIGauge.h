#pragma once
#include "UIWidget.h"

class UIGauge
	: public UIWidget
{
public:
	explicit UIGauge(Object* _owner);
	virtual ~UIGauge();
public:
	void Start() override;
	void PreUpdate() override;
	void Update() override;
	void EditorUpdate() override;
	void EditorGlobalUpdate() override;
	void EditorRendering(EditorViewerType _viewerType) override;
public:
	virtual void DrawWidget(Vector2 _scale)	override;
	eButtonState GetState() { return mButtonState; }
	void SetState(eButtonState _state) { mButtonState = _state; }
public:
	virtual void _CALLBACK OnEnable();
	virtual void _CALLBACK OnDisable();
	virtual void _CALLBACK OnDestroy();
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
private:
	ResourceHandle mBackgroundTextureHandle;
	ResourceHandle mTextureHandle;
	ResourceHandle mSelectedTextureHandle;

	Texture2DResource* m_pDefaultTexture = nullptr;
	Texture2DResource* m_pSelectedTexture = nullptr;

	eButtonState mButtonState = eButtonState::DEFAULT;

	Vector2 defaultPos = {};
	Vector2 selectedPos = {};
};

