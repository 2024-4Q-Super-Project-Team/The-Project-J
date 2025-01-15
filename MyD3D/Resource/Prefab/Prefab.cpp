#include "pch.h"
#include "Prefab.h"
#include "Object/Object.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"

Prefab::Prefab(std::wstring_view _name)
    : Resource(_name)
{
    Object* mainObject = new Object(_name.data(), L"");
    AddObject(mainObject);
}

Prefab::Prefab(std::wstring_view _name, std::shared_ptr<FBXModelResource> _pModel)
    : Resource(_name)
{
    Object* mainObject = new Object(_name.data(), L"");
    AddObject(mainObject);
    ModelNode* rootNode = _pModel->mRootNode;
    AddObjectFromNode(mainObject, rootNode);
    SetComponent(mainObject, _pModel);
}

Prefab::~Prefab()
{
    SAFE_DELETE_ARRAY(mObjectList);
}

void Prefab::AddObject(Object* _pObject)
{
    mObjectList.push_back(_pObject);
    mObjectTable[_pObject->GetName()] = _pObject;
}

void Prefab::AddObjectFromNode(Object* _pObject, ModelNode* _pNode)
{
    Object* pObject = new Object(_pNode->mNodeName, L"");
    // 1. Ʈ�������� ���� �������� ����
    pObject->transform->SetParent(_pObject->transform);
    // 2. Ʈ�������� ��� ����� ����
    pObject->transform->position = _pNode->mOrginPosition;
    pObject->transform->rotation = _pNode->mOrginRotatin;
    pObject->transform->scale    = _pNode->mOrginScale;
    // 3. �����տ� ���� ������Ʈ�� �߰�
    AddObject(pObject);
    // 4. ������ ��ȸ�ϱ� ���� ��������� �ݺ�
    for (int i = 0; i < _pNode->mChildren.size(); ++i)
    {
        AddObjectFromNode(pObject, _pNode->mChildren[i]);
    }
}


Object* Prefab::GetObjectFromName(const std::wstring& _name)
{
    auto itr = Helper::FindMap(_name, mObjectTable);
    if (itr == nullptr)
    {
        return nullptr;
    }
    else
    {
        return *itr;
    }
}

void Prefab::SetGroupName(const std::wstring& _groupName)
{
    mGroupName = _groupName;
}

void Prefab::SetComponent(Object* _pObject, std::shared_ptr<FBXModelResource> _pModel)
{
    SetMeshRenderer(_pModel->mRootNode, _pModel);
}

void Prefab::SetMeshRenderer(ModelNode* _pNode, std::shared_ptr<FBXModelResource> _pModel)
{
    for (int i = 0; i < _pNode->mMeshResources.size(); ++i)
    {
        // 0. ����� �̸��� �´� ������Ʈ�� �˻��Ѵ�.
        auto ppObject = Helper::FindMap(_pNode->mNodeName, mObjectTable);
        if (ppObject)
        {
            // 1. ����� �޽� Array�� ���� Mesh�� Material Ű ���� ������.
            std::wstring mMeshKey = _pNode->mMeshResources[i];
            std::wstring mMaterialKey = _pNode->mMatResources[i];
            // 2. Model�� ���ҽ� ������ ������ Key���� ���� ���ҽ��� ������.
            auto ppMesh     = Helper::FindMap(mMeshKey, _pModel->mMeshTable);
            auto ppMaterial = Helper::FindMap(mMaterialKey, _pModel->mMaterialTable);
            // 3. Mesh�� �� ��Ű�� ������ �ִ��� ������ Ȯ���ؼ� ������Ʈ�� �°� �߰��Ѵ�.
            if (ppMesh)
            {
                // ���� ������ MeshRenderer������Ʈ
                if ((*ppMesh)->mBoneArray.empty())
                {
                    MeshRenderer* pRenderer = (*ppObject)->AddComponent<MeshRenderer>();
                    // �޽��� �־��ش�.
                    pRenderer->SetMesh((*ppMesh));
                    // ��Ƽ������ ���� ���ɼ��� �ִ�. ���� ����ִ��� Ȯ��
                    if (ppMaterial)
                    {
                        pRenderer->SetMaterial(*ppMaterial);
                    }
                }
                // ���� ������ SkinnedMeshRenderer������Ʈ
                else
                {
                    SkinnedMeshRenderer* pRenderer = (*ppObject)->AddComponent<SkinnedMeshRenderer>();
                    // ��Ų�� �޽� �������� ��Ʈ ������ ����� ������Ʈ
                    auto** ppRootBone = Helper::FindMap(_pModel->mRootNode->mNodeName, mObjectTable);
                    pRenderer->SetMesh((*ppMesh));
                    if (ppRootBone)
                    {
                        pRenderer->SetRootBone((*ppRootBone)->transform);
                    }
                    if(ppMaterial)
                    {
                        pRenderer->SetMaterial(*ppMaterial);
                    }
                }
            }
        }
    }
    // 5. �ڽ� ��嵵 ��ȸ�ϸ� �߰��Ѵ�.
    for (auto& node : _pNode->mChildren)
    {
        SetMeshRenderer(node, _pModel);
    }
}

void Prefab::EditorRendering()
{
	for (auto& object : mObjectList)
	{
		object->EditorRendering();
	}
}

