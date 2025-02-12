#include "pch.h"
#include "World.h"
#include "World/WorldManager.h"
#include "Manager/GameManager.h"
#include "Physics/PhysicsManager.h"
#include "ViewportScene/ViewportScene.h"
#include "ViewportScene/ViewportManager.h"
#include "Object/Object.h"
#include "Component/Camera/Camera.h"

PxFilterFlags CustomFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;
    pairFlags |= PxPairFlag::eTRIGGER_DEFAULT;

    return PxFilterFlag::eDEFAULT;
}

World::World(ViewportScene* _pViewport, std::wstring_view _name, std::wstring_view _tag, bool isEmpty)
    : Entity(_name, _tag)
    , mOwnerScene(_pViewport)
{
    SetEID(Helper::ToString(_name.data()));
    mNeedResourceHandleTable.reserve(30);

    PxSceneDesc sceneDesc(GameManager::GetPhysicsManager()->GetPhysics()->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.f, -98.f, 0.f);
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.filterShader = CustomFilterShader;
    sceneDesc.simulationEventCallback = GameManager::GetPhysicsManager()->GetCallback();
   
    mPxScene = GameManager::GetPhysicsManager()->GetPhysics()->createScene(sceneDesc);

    mControllerManager = PxCreateControllerManager(*mPxScene);

}

World::~World()
{
    for (auto& obj : mObjectArray)
    {
        obj->transform->SetParent(nullptr);
    }

    SAFE_DELETE_VECTOR(mObjectArray);
    
    if (mControllerManager)
    {
        mControllerManager->release();
        mControllerManager = nullptr;
    }

    if (mPxScene)
    {
        mPxScene->release();
        mPxScene = nullptr;
    }
    if (Editor::InspectorViewer::IsFocusObject(this))
        Editor::InspectorViewer::SetFocusObject(nullptr);


}

void World::Start()
{
    for (int i = 0; i < mObjectArray.size(); i++)
    {
        mObjectArray[i]->Start();
    }
}

void World::Tick()
{
    UpdateObject();
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Tick();
        }
    }
}

void World::FixedUpdate()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->FixedUpdate();
        }
    }
}

void World::PreUpdate()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PreUpdate();
        }
    }
}

void World::Update()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Update();
        }
    }
}

void World::PostUpdate()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PostUpdate();
        }
    }
    if (mPxScene)
    {
        mPxScene->simulate(Time::GetUnScaledDeltaTime());
        mPxScene->fetchResults(true);
        mPxScene->fetchResultsParticleSystem();
    }
}

void World::PreRender()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PreRender();
        }
    }

}

void World::Render()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Render();
        }
    }
}

void World::Draw(Camera* _camera)
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->Draw(_camera);
        }
    }
}

void World::PostRender()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->PostRender();
        }
    }
}

void World::EditorUpdate()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->EditorUpdate();
        }
    }
}

void World::EditorGlobalUpdate()
{
    for (Object* object : mObjectArray)
    {
        if (object->transform->GetParent() == nullptr)
            object->EditorGlobalUpdate();
    }
}

void World::EditorRender()
{
    for (Object* object : mObjectArray)
    {
        if (object->GetState() == EntityState::Active)
        {
            if (object->transform->GetParent() == nullptr)
                object->EditorRender();
        }
    }
}

Object* World::CreateObject(std::wstring_view _name, std::wstring_view _tag)
{
    Object* instance = new Object(_name, _tag);
    instance->SetCreate();
    instance->SetWorld(this);
    return instance;
}

Object* World::FindObject(std::wstring_view _name)
{
    auto itr = FIND_CONTAINER(mObjectArray,
        [&_name](Object* _object) {
            return (_object->GetName() == _name.data());
        });
    return (FIND_SUCCESS(itr, mObjectArray)) ? (*itr) : nullptr;
}

void World::ShiftObject(Object* _dstObject)
{
    if(_dstObject->GetOwnerWorld()) 
    {
        World* dstWorld = _dstObject->GetOwnerWorld();
        auto itr = FIND_CONTAINER(dstWorld->mObjectArray,
            [&_dstObject](Object* _object) {
                return (_object == _dstObject);
            });
        if (FIND_SUCCESS(itr, dstWorld->mObjectArray))
        {
            dstWorld->mObjectArray.erase(itr);
        }
    }
    mObjectArray.push_back(_dstObject);
}

json World::Serialize()
{
    json ret;
    ret["id"] = GiveId();
    ret["name"] = Helper::ToString(mName);
    ret["is persistnace"] = isPersistance;
    json objs = json::array();
    for (auto& object : mObjectArray)
    {
        json objectJson;
        objectJson["id"] = object->GiveId();
        objectJson["name"] = Helper::ToString(object->GetName());
        objectJson["tag"] = Helper::ToString(object->GetName());
        objs += objectJson;
    }
    ret["objects"] = objs;

    return ret;
}

void World::Deserialize(json& j)
{
    for (auto& objectJson : j["objects"])
    {
        std::wstring name = Helper::ToWString(objectJson["name"].get<std::string>());
        std::wstring tag = Helper::ToWString(objectJson["tag"].get<std::string>());
        Object* instance = new Object(name, tag);
        instance->SetId(objectJson["id"].get<unsigned int>());
        instance->SetWorld(this);
    }
}

void _CALLBACK World::OnEnable()
{
    for (auto& object : mObjectArray)
    {
        object->OnEnable();
    }
    return void _CALLBACK();
}

void _CALLBACK World::OnDisable()
{
    for (auto& object : mObjectArray)
    {
        object->OnDisable();
    }
    return void _CALLBACK();
}

void World::InitWorldObject()
{
    {
        Object* mainCamera = CreateObject(L"Main_Camera", L"Default");
        Camera* cameraComponent = mainCamera->AddComponent<Camera>();
    }
    {
        Object* mainLight = CreateObject(L"Direction_Light", L"Default");
        Light* lightComponent = mainLight->AddComponent<Light>();
        Vector4 defaultDirection = Vector4(0.0f, 1.0f, 1.0f, 1.0f);
        defaultDirection.Normalize();
        lightComponent->GetProperty().Direction = defaultDirection;
    }
}

void World::UpdateObject()
{
    // 昏力 棺 积己 贸府
    for (auto itr = mObjectArray.begin(); itr != mObjectArray.end();)
    {
        // 昏力
        if ((*itr)->GetState() == EntityState::Destroy)
        {
            if (EditorManager::mInspectorViewer &&
                EditorManager::mInspectorViewer->GetFocusObject() == *itr) {
                EditorManager::mInspectorViewer->SetFocusObject(nullptr);
            }
            SAFE_DELETE(*itr);
            itr = mObjectArray.erase(itr);
            continue;
        }
        // 积己
        if ((*itr)->GetState() == EntityState::Create)
        {
            (*itr)->SetActive(true);
        }
        ++itr;
    }
}

void World::EditorRendering(EditorViewerType _viewerType)
{
    std::string name = Helper::ToString(GetName());
    std::string ptr = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    switch (_viewerType)
    {
        case EditorViewerType::DEFAULT:
        {
            std::string widgetID = name + ptr;
            if (mOwnerScene->GetWorldManager()->GetActiveWorld() == this)
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_WORLD_SELECTED);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_WORLD);
            }
            
            if (ImGui::Selectable((widgetID).c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight))
            {
                if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    Editor::InspectorViewer::SetFocusObject(this);
                }
                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    isNodeOpen = isNodeOpen == true ? false : true;
                }
            }
            EDITOR_COLOR_POP(1);
            break;
        }
        case EditorViewerType::HIERARCHY:
        {
            break;
        }
        case EditorViewerType::INSPECTOR:
        {
            {
                ImGui::Text("World");
                ImGui::Separator();

                if (ImGui::Checkbox(("IsPersistance" + ptr).c_str(), &isPersistance));
                ImGui::Separator();

                static char buffer1[128] = "";
                strcpy_s(buffer1, Helper::ToString(GetName()).c_str());
                if (ImGui::InputText((ptr + "InputName").c_str(), buffer1, IM_ARRAYSIZE(buffer1))) {
                    std::wstring newName = Helper::ToWString(std::string(buffer1));
                    SetName(newName);
                }
                static char buffer2[128] = "";
                strcpy_s(buffer2, Helper::ToString(GetTag()).c_str());
                ImGui::Text("Tag ");
                if (ImGui::InputText((ptr + "Tag").c_str(), buffer2, IM_ARRAYSIZE(buffer2))) {
                    std::wstring newTag = Helper::ToWString(std::string(buffer2));
                    SetTag(newTag);
                }
            }
            ImGui::Separator();
            {
                ImGui::Text("Pre Load Resource List :");
                for (auto itr = mNeedResourceHandleTable.begin(); itr != mNeedResourceHandleTable.end(); ++itr)
                {
                    ImGui::Text(Helper::ToString(*itr).c_str());
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}