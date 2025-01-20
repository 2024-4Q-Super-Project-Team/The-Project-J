#pragma once
#include "Resource/Resource.h"

class World;
class VertexShader;
class PixelShader;
class FBXModelResource;
class Texture2D;
class Camera;

class SkyBox
    : public Resource
{
public:
    RESOURCE_TYPE(SkyBox);
    explicit SkyBox(ResourceHandle _handle);
    virtual ~SkyBox() = default;
public:
    void Draw(Matrix& _view, Matrix& _projection, FLOAT _far = 10000.0f);
public:
    void SetEnvironmentTexture(std::shared_ptr<Texture2D> _tex);
    void SetDiffuseTexture(std::shared_ptr<Texture2D> _tex);
    void SetSpecularture(std::shared_ptr<Texture2D> _tex);
    void SetLookUpTableTexture(std::shared_ptr<Texture2D> _tex);
private:
    std::shared_ptr<Texture2D>  mIBLEnvironmentTex;
    std::shared_ptr<Texture2D>  mIBLDiffuseTex;
    std::shared_ptr<Texture2D>  mIBLSpecularTex;
    std::shared_ptr<Texture2D>  mBLDFLookUpTableTex;
public:
    static std::shared_ptr<SkyBox> DefaultSkyBox;
    static void InitSkyBoxResource();
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

