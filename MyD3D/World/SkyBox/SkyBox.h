#pragma once

class World;
class VertexShader;
class PixelShader;
class FBXModelResource;
class Texture2D;
class Camera;

class SkyBox
{
public:
    explicit SkyBox(World* _pOwnerWorld);
    virtual ~SkyBox() = default;
public:
    void Draw(Camera* _camera);
    void Bind();
public:
    void SetEnvironmentTexture(std::shared_ptr<Texture2D> _tex);
    void SetDiffuseTexture(std::shared_ptr<Texture2D> _tex);
    void SetSpecularture(std::shared_ptr<Texture2D> _tex);
    void SetLookUpTableTexture(std::shared_ptr<Texture2D> _tex);
private:
    World* mOwnerWorld;
    D3DGraphicsVertexShader* mSkyBoxVS;
    D3DGraphicsPixelShader* mSkyBoxPS;
    std::shared_ptr<Texture2D>  mIBLEnvironmentTex;
    std::shared_ptr<Texture2D>  mIBLDiffuseTex;
    std::shared_ptr<Texture2D>  mIBLSpecularTex;
    std::shared_ptr<Texture2D>  mBLDFLookUpTableTex;

};

