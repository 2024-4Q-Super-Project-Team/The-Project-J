#pragma once
#include "Component/Component.h"

class Effect : public Component, public IRenderContext
{
public:
	explicit Effect(Object* _owner);
	virtual ~Effect();
public:
	virtual void Start();
	virtual void Tick();
	virtual void FixedUpdate();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render();
	virtual void Draw(Camera* _camera);
	virtual void PostRender();
	// Editor Only
	virtual void EditorUpdate();
	virtual void EditorRender();
public:
	virtual json Serialize();
	virtual void Deserialize(json& j);
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override; 

public:
	virtual eBlendModeType GetBlendMode() override { return eBlendModeType::PARTICLE_BLEND; }
	virtual eRasterizerStateType GetCullingMode() override { return eRasterizerStateType::NONE_CULLING; }
	virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
	virtual void DrawObject(Matrix& _view, Matrix& _projection) override;
	virtual void DrawShadow(Light* _pLight) override;
	virtual void DrawWire() override;
	virtual void EditorGlobalUpdate() override;

	virtual void _CALLBACK OnEnable() override;
public:
	void SetTexture(ResourceHandle _handle);
private:
	float mPosition[4] = { 0,0,0,1 };
	ResourceHandle mTextureHandle;
	Texture2DResource* mTexture;
	float mSize = 1.f;

	class D3DGraphicsVertexBuffer* mVertexBuffer = nullptr;

	BoundingOrientedBox mDebugBox;

};

