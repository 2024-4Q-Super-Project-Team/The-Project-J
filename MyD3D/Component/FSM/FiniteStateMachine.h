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
private:
    void UpdateState();
public:
    template<typename T>
    void AddState(const std::string& _key)
    {
    }
    template<typename T>
    T* GetState(const std::string& _key)
    {
    }
    void ChangeState(const std::string& _key);
private:
    std::unordered_map<std::wstring, FSMState*> mStateTable;
    FSMState* mCurState = nullptr;
    FSMState* mPreState = nullptr;
};

