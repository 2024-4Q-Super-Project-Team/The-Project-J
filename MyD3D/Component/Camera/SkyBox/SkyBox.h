#pragma once

class World;
class VertexShader;
class PixelShader;
class Texture2DResource;
class Camera;

class SkyBox : public IEditorObject
{
    enum eSkyBoxTextureType
    {
        Environment,
        Diffuse,
        Specular,
        BLDFLookUpTable,
        SKYBOX_TEX_SIZE,
    };
public:
    explicit SkyBox();
    virtual ~SkyBox() = default;
public:
    void Draw(Matrix& _view, Matrix& _projection, FLOAT _far = 10000.0f);
public:
    void SetEnvironmentTexture(ResourceHandle _handle);
    void SetDiffuseTexture(ResourceHandle _handle);
    void SetSpecularture(ResourceHandle _handle);
    void SetLookUpTableTexture(ResourceHandle _handle);
private:
    ResourceHandle     mSkyBoxTexHandle[SKYBOX_TEX_SIZE];
    Texture2DResource* mSkyBoxTexture2D[SKYBOX_TEX_SIZE] = { nullptr, };
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
    static SkyBox* DefaultSkyBox;
    static SkyBox* GetDefaultSkyBox();
    static void    FreeDefaultSkyBox();
};

