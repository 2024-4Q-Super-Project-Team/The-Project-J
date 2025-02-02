#pragma once
#include "UIWidget.h"

class UISprite
	: public UIWidget
{
public:
	explicit UISprite(Object* _owner);
	virtual ~UISprite();
public:
	void PreUpdate() override;
	void Update() override;
	void EditorUpdate() override;
	void EditorGlobalUpdate() override;
	void EditorRendering(EditorViewerType _viewerType) override;
public:
	virtual void DrawWidget(Vector2 _scale)	override;
public:
	virtual void _CALLBACK OnEnable();
	virtual void _CALLBACK OnDisable();
	virtual void _CALLBACK OnDestroy();
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
private:
	ResourceHandle mTextureHandle;
};

