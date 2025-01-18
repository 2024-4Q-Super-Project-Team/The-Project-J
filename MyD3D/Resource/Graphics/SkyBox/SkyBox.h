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
    explicit SkyBox(std::wstring_view _name);
    virtual ~SkyBox() = default;
public:
    void Draw(Camera* _camera);
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
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};

