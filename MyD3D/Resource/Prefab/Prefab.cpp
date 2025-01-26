#include "pch.h"
#include "Prefab.h"
#include "Object/Object.h"
#include "Resource/Graphics/FBXModel/FBXModel.h"
#include "Resource/Graphics/Mesh/Mesh.h"
#include "Resource/Graphics/Material/Material.h"

#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"
#include "World/WorldManager.h"
#include "World/World.h"

PrefabResource::PrefabResource(ResourceHandle _handle)
    : Resource(_handle)
{
    SetEID("Prefab : " + Helper::ToString(_handle.GetKey()));
    Object* mainObject = new Object(GetKey(), L"");
    AddObject(mainObject);
}

PrefabResource::PrefabResource(ResourceHandle _handle, FBXModelResource* _pModel)
    : Resource(_handle)
{
    SetEID("Prefab : " + Helper::ToString(_handle.GetKey()));
    Object* mainObject = new Object(GetKey(), L"");
    AddObject(mainObject);
    if (_pModel)
    {
        ModelNode* rootNode = _pModel->mRootNode;
        if (rootNode)
        {
            AddObjectFromNode(mainObject, rootNode);
            SetComponent(mainObject, _pModel);
        }
    }
}

PrefabResource::~PrefabResource()
{
    SAFE_DELETE_ARRAY(mObjectList);
}

void PrefabResource::AddObject(Object* _pObject)
{
    mObjectList.push_back(_pObject);
    mObjectTable[_pObject->GetName()] = _pObject;
}

void PrefabResource::AddObjectFromNode(Object* _pObject, ModelNode* _pNode)
{
    Object* pObject = new Object(_pNode->mNodeName, L"");
    // 1. Ʈ�������� ���� �������� ����
    pObject->transform->SetParent(_pObject->transform);
    // 2. Ʈ�������� ��� ����� ����
    pObject->transform->position = _pNode->mOrginPosition;
    pObject->transform->rotation = _pNode->mOrginRotatin;
    pObject->transform->scale = _pNode->mOrginScale;
    // 3. �����տ� ���� ������Ʈ�� �߰�
    AddObject(pObject);
    // 4. ������ ��ȸ�ϱ� ���� ��������� �ݺ�
    for (int i = 0; i < _pNode->mChildren.size(); ++i)
    {
        AddObjectFromNode(pObject, _pNode->mChildren[i]);
    }
}


Object* PrefabResource::GetObjectFromName(const std::wstring& _name)
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

Object* PrefabResource::Instantiate(World* _dstWorld)
{
    World* dstWorld = _dstWorld == nullptr ? GameManager::GetCurrentWorld() : _dstWorld;
   
    if (dstWorld)
    {
        std::list<Object*>                          cloneArray;
        std::unordered_map<std::wstring, Object*>   cloneTable;

        cloneTable.reserve(mObjectTable.size());

        for (auto itr = mObjectList.begin(); itr != mObjectList.end(); ++itr)
        {
            Object* clone = dstWorld->CreateObject((*itr)->GetName(), (*itr)->GetTag());
            cloneTable[clone->GetName()] = clone;
            cloneArray.push_back(clone);
        }

        auto clone = cloneArray.begin();
        auto PrefabResource = mObjectList.begin();

        // ������ ������Ʈ Clone(������Ʈ)
        while (clone != cloneArray.end())
        {
            (*PrefabResource)->Clone((*clone), cloneTable);
            clone++;
            PrefabResource++;
        }
        return cloneArray.front();
    }
    return nullptr;
}

void PrefabResource::SetComponent(Object* _pObject, FBXModelResource* _pModel)
{
    SetMeshRenderer(_pModel->mRootNode, _pModel);
}

void PrefabResource::SetMeshRenderer(ModelNode* _pNode, FBXModelResource* _pModel)
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
            auto ppMesh = Helper::FindMap(mMeshKey, _pModel->mMeshTable);
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
                    if (ppMaterial)
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

void PrefabResource::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetKey());

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable((GetEID() + uid).c_str(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = name;
        EditorDragNDrop::SendDragAndDropData(GetEID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR:
    {
        {   // Clone Button
            ImVec2 buttonSize = ImVec2(120, 30); // ��ư ũ�� 
            if (ImGui::Button(("Clone" + uid).c_str(), buttonSize))
            {
                Instantiate(
                    EditorManager::mFocusViewport->GetWorldManager()->GetActiveWorld()
                );
            }
        }
        break;
    }
    default:
        break;
    }
}

