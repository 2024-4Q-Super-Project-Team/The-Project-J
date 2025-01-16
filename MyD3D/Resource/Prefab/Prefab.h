#pragma once
#include "Resource/Resource.h"

class Object;
class FBXModelResource;
class ModelNode;

class Prefab
    : public Resource
{
public:
    RESOURCE_TYPE(Prefab);
    explicit Prefab(std::wstring_view _name);
    explicit Prefab(std::wstring_view _name, std::shared_ptr<FBXModelResource> _pModel);
    virtual ~Prefab();
public:
    Object* GetObjectFromName(const std::wstring& _name);
    void    SetGroupName(const std::wstring& _groupName);  
private:
    // �׷� �̸�. ���߿� Ŭ�� ������ �ش� �׷����� ������. ������ ����
    std::wstring mGroupName;
    std::list<Object*> mObjectList;
    std::unordered_map<std::wstring, Object*> mObjectTable;
    friend class ResourceManager;
    friend class MonoBehaviour;
private:
    void AddObject(Object* _pObject);
    void AddObjectFromNode(Object* _pObject, ModelNode* _pNode);
    void SetComponent(Object* _pObject, std::shared_ptr<FBXModelResource> _pModel);
    void SetMeshRenderer(ModelNode* _pNode, std::shared_ptr<FBXModelResource> _pModel);
public:
	virtual void EditorRendering() override;
};