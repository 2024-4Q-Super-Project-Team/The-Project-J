#pragma once

#include "Component/Component.h"
#include "World/World.h"

class UIWidget;
class Camera;
class Transform;

class Canvas
    : public Component
{
public:
    explicit Canvas(Object* _owner);
    virtual ~Canvas();
public:
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
	virtual void EditorUpdate()			override;
	virtual void EditorGlobalUpdate()	override;
	virtual void EditorRender()			override;
	virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
	virtual void _CALLBACK OnEnable();
	virtual void _CALLBACK OnDisable();
	virtual void _CALLBACK OnDestroy();
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
public:
    void PushDrawList();
    // À§Á¬ ÄÄÆ÷³ÍÆ®
    std::vector<UIWidget*> mWidgets;
};

