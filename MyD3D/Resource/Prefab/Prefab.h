#pragma once
#include "Resource/Resource.h"

class Object;
class FBXModelResource;
class ModelNode;
class World;

class PrefabResource : public Resource
{
public:
    RESOURCE_TYPE(PrefabResource);
    explicit PrefabResource(ResourceHandle _handle);
    explicit PrefabResource(ResourceHandle _handle, FBXModelResource* _pModel);
    virtual ~PrefabResource();
public:
    Object* GetObjectFromName(const std::wstring& _name);
public:
    Object* Instantiate(World* _dstWorld = nullptr);
private:
    std::list<Object*> mObjectList;
    std::unordered_map<std::wstring, Object*> mObjectTable;
    friend class ResourceManager;
    friend class MonoBehaviour;
private:
    void AddObject(Object* _pObject);
    void AddObjectFromNode(Object* _pObject, ModelNode* _pNode);
    void SetComponent(Object* _pObject, FBXModelResource* _pModel);
    void SetMeshRenderer(ModelNode* _pNode, FBXModelResource* _pModel);
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};