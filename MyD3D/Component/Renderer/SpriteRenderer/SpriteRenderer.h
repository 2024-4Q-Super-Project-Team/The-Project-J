#pragma once
#include "Component/Renderer/Renderer.h"

class Texture2DResource;

class SpriteRenderer
    : public Component, public IRenderContext
{
    struct Vertex
    {
        Vector4 pos;
        Vector2 uv;
    };
public:
	explicit SpriteRenderer(Object* _owner);
	virtual ~SpriteRenderer();
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
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    virtual void DrawObject(Matrix& _view, Matrix& _projection) override;
    virtual void DrawShadow(Light* _pLight) override;
    virtual void DrawWire() override;
public:
    virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
    virtual eBlendModeType GetBlendMode() override;
    virtual eRasterizerStateType GetCullingMode() override;
public:
    void SetSprite(ResourceHandle _handle);
    void SetTileIndexWidth(UINT _numWidth);
    void SetTileIndexHeight(UINT _numHeight);
    void SetIndex(UINT _index);
    void SetIndex(UINT _width, UINT height);

    int GetFrameCount() { return mFrameCount; }
public:
    ResourceHandle      mSpriteHandle;
    Texture2DResource*  mSpriteTexture;

    INT mTileIndexWidth = 1;
    INT mTileIndexHeight = 1;
    INT mTileIndex = 0;
    INT mTileSize = 20;
    INT mFrameCount = 1;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
};

