#pragma once
#include "FSMState/FSMState.h"

class FSMState;

class FiniteStateMachine 
	: public Component
{
public:
    explicit FiniteStateMachine(Object* _owner);
    virtual ~FiniteStateMachine();
public:
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera) override;
    virtual void PostRender() override;
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorRender() override;
public:
    virtual void _CALLBACK OnEnable() {};
    virtual void _CALLBACK OnDisable() {};
    virtual void _CALLBACK OnDestroy() {};
private:
    void UpdateState();
public:
    void ChangeState(const std::wstring& _key);
    template<typename T>
    T* AddState(const std::wstring& _key)
    {
        T* instance;
        auto itr = mStateTable.find(_key);
        if (FIND_FAILED(itr, mStateTable))
        {
            instance = new T(this, gameObject);
            mStateTable[_key] = instance;
            instance->Start();
        }
        else
        {
            instance = dynamic_cast<T*>(itr->second);
        }
        return instance;
    }
    template<typename T>
    T* GetState(const std::wstring& _key)
    {
        auto itr = mStateTable.find(_key);
        if (FIND_FAILED(itr, mStateTable))
        {
            return nullptr;
        }
        return dynamic_cast<T*>(itr->second);
    }
private:
    std::unordered_map<std::wstring, FSMState*> mStateTable;
    FSMState* mCurrState = nullptr;
    FSMState* mNextState = nullptr;
public:
    virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
};

