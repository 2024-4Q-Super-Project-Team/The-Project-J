#include "pch.h"
#include "Object.h"
#include "World/World.h"
#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"
#include "Editor/CumstomWidget/TabItem/Viewer/InspectorViewer/EditorInspectorViewer.h"

Object::Object(std::wstring_view _name, std::wstring_view _tag)
    : Engine::Entity(_name, _tag)
    , transform(new Transform(this))
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)].push_back(transform);
}

Object::~Object()
{
    for (auto& compArr : mComponentArray)
    {
        SAFE_DELETE_VECTOR(compArr);
    }
    if (Editor::InspectorViewer::IsFocusObject(this))
        Editor::InspectorViewer::SetFocusObject(nullptr);
    if (Editor::GuizmoManipulater::IsFocusObject(this))
        Editor::GuizmoManipulater::SetFocusObjedct(nullptr);
}

Object::Object(const Object& _other)
    : Engine::Entity(_other.mName, _other.mTag)
    , transform(new Transform(this))
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)][0] = transform;
}

void Object::Start()
{
    for (int i = 0; i < (UINT)eComponentType::SIZE; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            comp->Start();
        }
    }
}

void Object::Tick()
{    
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Tick();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->Tick();
        }
    }
}

void Object::FixedUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->FixedUpdate();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->FixedUpdate();
        }
    }
}

void Object::PreUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PreUpdate();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->PreUpdate();
        }
    }
}

void Object::Update()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Update();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->Update();
        }
    }
}

void Object::PostUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PostUpdate();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->PostUpdate();
        }
    }
}

void Object::PreRender()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PreRender();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->PreRender();
        }
    }
}

void Object::Render()
{
    if (!transform->GetParent())
    {
        transform->UpdateMatrix();
    }
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Render();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->Render();
        }
    }
}

void Object::Draw(Camera* _camera)
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Draw(_camera);
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->Draw(_camera);
        }
    }
}

void Object::PostRender()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->PostRender();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->PostRender();
        }
    }
}

// 에디터 업데이트는 활성화 여부와 관계없이 업데이트 되어야 한다
void Object::EditorUpdate()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            comp->EditorUpdate();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->EditorUpdate();
        }
    }
    if (!transform->GetParent())
        transform->UpdateMatrix();
}

void Object::EditorRender()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            comp->EditorRender();
        }
    }
    const std::vector<Transform*>& children = transform->GetChildren();
    for (Transform* child : children)
    {
        if (child->gameObject->GetState() == EntityState::Active)
        {
            child->gameObject->EditorRender();
        }
    }
}

void _CALLBACK Object::OnEnable()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->OnEnable();
        }
    }
    return void _CALLBACK();
}

void _CALLBACK Object::OnDisable()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->OnDisable();
        }
    }
    return void _CALLBACK();
}

void _CALLBACK Object::OnDestroy()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetDestroy();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
    {
        static_cast<MonoBehaviour*>(comp)->OnDestroy();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
    {
        static_cast<MonoBehaviour*>(comp)->OnDestroy();
    }
    return void _CALLBACK();
}

void Object::Clone(Object* _pDest, std::unordered_map<std::wstring, Object*>& _objTable)
{
    for (auto& compArr : mComponentArray)
    {
        for (auto& comp : compArr)
        {
            comp->Clone(_pDest, _objTable);
        }
    }
}

std::vector<Component*> Object::GetAllComponents()
{
    std::vector<Component*> ret;
    for (auto& typeComps : mComponentArray)
    {
        for (auto& comp : typeComps)
        {
            ret.push_back(comp);
        }
    }
    return ret;
}

void Object::SetWorld(World* _world)
{
    if (_world)
    {
        if (mOwnerWorld != _world)
        {
            _world->ShiftObject(this);
            mOwnerWorld = _world;
        }
    }
}

json Object::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = Helper::ToString(mName);
    ret["tag"] = Helper::ToString(mTag);

    json cmps = json::array();
    for (auto& cmpArr : mComponentArray)
    {
        for (auto& cmp : cmpArr)
        {
            json j;
            j["id"] = cmp->GiveId();
            j["name"] = typeid(*cmp).name() + sizeof("Class");
            cmps.push_back(j);
        }
    }
    ret["components"] = cmps;

    return ret;
}

void Object::Deserialize(json& j)
{
    mTag = Helper::ToWString(j["tag"].get<std::string>());

    for (auto& componentJson : j["components"])
    {
        std::string name = componentJson["name"].get<std::string>();
        Component* component;
        if (name == "Transform") //Transform은 기본 컴포넌트이므로 추가로 생성하지 않습니다. 
            component = transform;
        else
            component = static_cast<Component*>(CREATE_COMPONENT(name, this));

        if (component)
            component->SetId(componentJson["id"].get<unsigned int>());
    }
}

void Object::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetName());

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        auto flags = ImGuiSelectableFlags_Highlight | ImGuiSelectableFlags_AllowDoubleClick;
        bool isSelected = false;
        if (Editor::InspectorViewer::IsFocusObject(this))
        {
            isSelected = true;
        }
        std::string symbol = "::: ";
        if (transform->GetChildren().empty() == false)
        {
            symbol = isNodeOpen ? "+ " : "- ";
        }
        if (isSelected)
        {
            GetState() == EntityState::Passive ?
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_OBJECT_DISABLE_SELECTED) :
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_OBJECT_SELECTED);
        }
        else
        {
            GetState() == EntityState::Passive ?
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_OBJECT_DISABLE) :
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_OBJECT);
        }
        std::string widgetID = symbol + name + uid;
        if (ImGui::Selectable((widgetID).c_str(), isSelected, flags))
        {
            if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            {
                Editor::InspectorViewer::SetFocusObject(this);
                Editor::GuizmoManipulater::SetFocusObjedct(this);
            }
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                isNodeOpen = isNodeOpen == true ? false : true;
            }
        }
        EDITOR_COLOR_POP(1);
        /////////////////////////////////////////////////////////////////
        // Drag&Drop
        /////////////////////////////////////////////////////////////////
        EditorItemState state;
        state.mObjectPtr = this;
        state.mName = name;
        EditorDragNDrop::SendDragAndDropData((widgetID).c_str(), state);

        Object* receiveObject = nullptr;
        if (EditorDragNDrop::ReceiveDragAndDropObjectData((widgetID).c_str(), &receiveObject))
        {
            // 자기자신이 아니거나 대상 오브젝트가 내 자식에 속해있지 않는 경우
            if (receiveObject != this && 
                transform->IsBelong(receiveObject->transform) == false)
            {
                receiveObject->transform->SetParent(transform);
                isNodeOpen = true;
            }
        }
        break;
    } 
    case EditorViewerType::HIERARCHY:
        break;
    case EditorViewerType::INSPECTOR:
    {
        {   // 활성화 / 비활성화
            bool preState = mState == EntityState::Active ? true : false;
            bool isActive = preState;
            ImGui::Checkbox((uid + "IsActive").c_str(), &isActive);
            if (isActive != preState)
            {
                isActive == true ? OnEnable() : OnDisable();
                mState = isActive == true ? EntityState::Active : EntityState::Passive;
            }
            ImGui::SameLine();
            static char buffer1[128] = "";
            strcpy_s(buffer1, Helper::ToString(GetName()).c_str());
            if (ImGui::InputText((uid + "InputName").c_str(), buffer1, IM_ARRAYSIZE(buffer1))) {
                std::wstring newName = Helper::ToWString(std::string(buffer1));
                SetName(newName);
            }
            static char buffer2[128] = "";
            strcpy_s(buffer2, Helper::ToString(GetTag()).c_str());
            ImGui::Text("Tag "); ImGui::SameLine();
            if (ImGui::InputText((uid + "Tag").c_str(), buffer2, IM_ARRAYSIZE(buffer2))) {
                std::wstring newTag = Helper::ToWString(std::string(buffer2));
                SetTag(newTag);
            }
        }
        ImGui::Separator();
        {   // 컴포넌트 렌더링
            for (auto& componentVec : mComponentArray)
            {
                for (auto itr = componentVec.begin(); itr != componentVec.end();)
                {
                    Component* component = *itr;
                    std::string compUID = "##" + std::to_string(reinterpret_cast<uintptr_t>(component));
                    
                    bool isTreeOpen = ImGui::TreeNodeEx((component->GetEID() + compUID).c_str(), EDITOR_FLAG_MAIN);
                    
                    // 버튼을 오른쪽 끝에 배치
                    ImGui::SameLine(); // 같은 줄에 추가
                    float cursorX = ImGui::GetCursorPosX();
                    float contentWidth = ImGui::GetContentRegionAvail().x;

                    // 커서를 트리 노드 오른쪽 끝으로 이동
                    ImGui::SetCursorPosX(cursorX + contentWidth - 20); // 50은 버튼 크기 + 여백
                    if (ImGui::Button("*", ImVec2(20, 20))) // 설정 아이콘
                    {
                        // 버튼 클릭 시 동작
                        ImGui::OpenPopup("Component Config");
                    }
                    // 팝업 메뉴
                    if (ImGui::BeginPopup("Component Config"))
                    {
                        if (ImGui::Button("Remove Component", ImVec2(100,50)))
                        {
                            delete component;
                            itr = componentVec.erase(itr);
                            ImGui::EndPopup(); // 팝업 종료
                            if (isTreeOpen)
                                ImGui::TreePop();
                            continue; // 다음 반복으로 진행
                        }
                        ImGui::EndPopup();
                    }
                    if (isTreeOpen)
                    {
                        component->EditorRendering(EditorViewerType::INSPECTOR);
                        ImGui::TreePop();
                    }
                    ++itr;
                }
            }
            ImGui::Separator();
            {   // 컴포넌트 추가
                ImVec2 buttonSize = ImVec2(120, 30); // 버튼 크기 
                if (ImGui::Button(("Add Component" + uid).c_str(), buttonSize))
                {
                    ImGui::OpenPopup("Component List"); // 버튼 클릭 시 팝업 열기
                }
                if (ImGui::BeginPopup("Component List"))
                {
                    const std::vector<std::string_view>& names = ComponentFactory::GetComopnentNames();

                    for (const std::string_view& name : names)
                    {
                        if (ImGui::Selectable(name.data()))
                        {
                            CREATE_EDITOR_COMPONENT(name, this);
                            ImGui::CloseCurrentPopup();         // 팝업 닫기
                        }
                    }
                    // 팝업 내에서 마우스 오른쪽 클릭 시 팝업 닫기
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }
        }
        break;
    }
    default:
        break;
    }
}
