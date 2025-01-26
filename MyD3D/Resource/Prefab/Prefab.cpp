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
    // 1. 트랜스폼을 통해 계층구조 구성
    pObject->transform->SetParent(_pObject->transform);
    // 2. 트랜스폼에 노드 행렬을 적용
    pObject->transform->position = _pNode->mOrginPosition;
    pObject->transform->rotation = _pNode->mOrginRotatin;
    pObject->transform->scale = _pNode->mOrginScale;
    // 3. 프리팹에 현재 오브젝트를 추가
    AddObject(pObject);
    // 4. 계층을 순회하기 위해 재귀적으로 반복
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

        // 만들어둔 오브젝트 Clone(컴포넌트)
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
        // 0. 노드의 이름에 맞는 오브젝트를 검색한다.
        auto ppObject = Helper::FindMap(_pNode->mNodeName, mObjectTable);
        if (ppObject)
        {
            // 1. 노드의 메쉬 Array를 통해 Mesh와 Material 키 값을 가져옴.
            std::wstring mMeshKey = _pNode->mMeshResources[i];
            std::wstring mMaterialKey = _pNode->mMatResources[i];
            // 2. Model의 리소스 내에서 각각의 Key값을 통해 리소스를 가져옴.
            auto ppMesh = Helper::FindMap(mMeshKey, _pModel->mMeshTable);
            auto ppMaterial = Helper::FindMap(mMaterialKey, _pModel->mMaterialTable);
            // 3. Mesh의 본 스키닝 정보가 있는지 없는지 확인해서 컴포넌트를 맞게 추가한다.
            if (ppMesh)
            {
                // 본이 없으면 MeshRenderer컴포넌트
                if ((*ppMesh)->mBoneArray.empty())
                {
                    MeshRenderer* pRenderer = (*ppObject)->AddComponent<MeshRenderer>();
                    // 메쉬를 넣어준다.
                    pRenderer->SetMesh((*ppMesh));
                    // 머티리얼은 없을 가능성이 있다. 따라서 비어있는지 확인
                    if (ppMaterial)
                    {
                        pRenderer->SetMaterial(*ppMaterial);
                    }
                }
                // 본이 있으면 SkinnedMeshRenderer컴포넌트
                else
                {
                    SkinnedMeshRenderer* pRenderer = (*ppObject)->AddComponent<SkinnedMeshRenderer>();
                    // 스킨드 메쉬 렌더러의 루트 본으로 사용할 오브젝트
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
    // 5. 자식 노드도 순회하며 추가한다.
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
            ImVec2 buttonSize = ImVec2(120, 30); // 버튼 크기 
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

