#include "pch.h"
#include "Object.h"
#include "ObjectGroup/ObjectGroup.h"

Object::Object(std::wstring_view _name, std::wstring_view _tag)
    : Engine::Entity(_name, _tag)
    , transform(new Transform(this))
    , mOwnerGroup(nullptr)
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)].push_back(transform);
}

Object::~Object()
{
    for (auto& compArr : mComponentArray)
    {
        SAFE_DELETE_VECTOR(compArr);
    }
}

Object::Object(const Object& _other)
    : Engine::Entity(_other.mName, _other.mTag)
    , transform(new Transform(this))
    , mOwnerGroup(_other.mOwnerGroup)
{
    mComponentArray[static_cast<UINT>(eComponentType::Transform)][0] = transform;
}

void Object::Tick()
{
    for (auto it = mComponentsToWake.begin(); it != mComponentsToWake.end(); )
    {
        Component* component = *it;
        if (component->IsActive() && component->IsAwake())
        {
            component->Start();
            int index = static_cast<UINT>(component->GetType());
            mComponentArray[index].push_back(component);
    
            it = mComponentsToWake.erase(it);
        }
        else ++it;
    }
    
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive() && comp->IsAwake())
                comp->Tick();
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
    if (!transform->GetParent())
        transform->UpdateMatrix();
}

void Object::Render()
{
    for (int i = 0; i < (UINT)eComponentType::UPDATE_END; ++i)
    {
        for (auto& comp : mComponentArray[i])
        {
            if (comp->IsActive())
                comp->Render();
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
}

void _CALLBACK Object::OnEnable()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetActive(true);
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
    {
        if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnEnable();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
    {
        if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnEnable();
    }
    return void _CALLBACK();
}

void _CALLBACK Object::OnDisable()
{
    for (auto child : transform->GetChildren())
    {
        child->gameObject->SetActive(false);
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::SCRIPT)])
    {
        if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnDisable();
    }
    for (auto comp : mComponentArray[Helper::ToInt(eComponentType::FINITE_STATE_MACHINE)])
    {
        if (comp->IsActive())
            static_cast<MonoBehaviour*>(comp)->OnDisable();
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
    for (auto& comp : mComponentsToWake)
    {
        comp->Clone(_pDest, _objTable);
    }

}

json Object::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = Helper::ToString(mName);

    json cmps = json::array();
    for (auto& cmpArr : mComponentArray)
    {
        for (auto& cmp : cmpArr)
        {
            json j;
            j["id"] = cmp->GetId();
            j["name"] = typeid(*cmp).name() + sizeof("Class");
            cmps.push_back(j);
        }
    }
    ret["components"] = cmps;

    return ret;
}

json Object::SerializeComponents()
{
    json ret;

    for (auto& cmpArr : mComponentArray)
    {
        for (auto& cmp : cmpArr)
        {
            ret["id"] = cmp->GetId();
            ret["data"] = cmp->Serialize();
        }
    }
    return ret;
}

void Object::Deserialize(json& j)
{
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

void Object::EditorRendering()
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetName());
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
        static char buffer[128] = "";
        strcpy_s(buffer, Helper::ToString(GetName()).c_str());
        if (ImGui::InputText((uid + "InputName").c_str(), buffer, IM_ARRAYSIZE(buffer))) {
            std::wstring newName = Helper::ToWString(std::string(buffer));
            SetName(newName);
        }
    }
    ImGui::Separator();
    {   // 컴포넌트 렌더링
        for (auto& componentVec : mComponentArray)
        {
            for (auto& component : componentVec)
            {
                component->EditorRendering();
            }
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
