#pragma once

class ModelSpawner : public MonoBehaviour
{
public:
    using MonoBehaviour::MonoBehaviour;
    virtual ~ModelSpawner();
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;

public:
    virtual json Serialize() override;
private:
    std::vector<std::shared_ptr<FBXModelResource>>  mFBXModelResources;
    std::vector<std::shared_ptr<Prefab>>            mModelPrefabs;
    std::vector<RendererComponent*>                 mRenderComponents;
    Object* mObject;

    Camera* mMainCamera;
};