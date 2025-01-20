#pragma once

class Component;

#include "../../Object/Object.h"

class ICreator
{
public:
	virtual ~ICreator() = default;
	virtual void* Create(Object* owner, bool isEditorMode, void* args = nullptr) const = 0;
};

template <typename T, typename... Args>
class Creator : public ICreator {
public:
    void* Create(Object* owner, bool isEditorMode, void* args = nullptr) const override {
        // args를 적절히 캐스팅하여 생성자에 전달
        if constexpr (sizeof...(Args) == 0) {
            return owner->AddComponent<T>(); // 인자가 없는 경우
        }
        else {
            return CallConstructor(owner, static_cast<std::tuple<Args...>*>(args));
        }
    }

private:
    template<typename Tuple>
    T* CallConstructor(Object* owner, Tuple* args) const
    {
        return std::apply([owner](Args... unpackedArgs) { return owner->AddComponent<T>(unpackedArgs...); }, *args);
    }
};

class ComponentFactory
{
public:
    ComponentFactory();
    ~ComponentFactory();

    static void Register(const std::string_view& name, ICreator* creator)
    {
        mFactoryMap[name] = creator;
        mComopnentNames.push_back(name);
    }

    static void* Create(const std::string_view& name, Object* owner, void* args = nullptr)
    {
        auto it = mFactoryMap.find(name);
        if (it != mFactoryMap.end())
            return it->second->Create(owner, args);
        return nullptr;
    }
    static std::vector<std::string_view>& GetComopnentNames()
    {
        return mComopnentNames;
    }
private:
    static std::unordered_map<std::string_view, ICreator*> mFactoryMap;

    static std::vector<std::string_view> mComopnentNames;
};


#define REGISTER_COMPONENT(Type)\
ComponentFactory::Register(#Type, new Creator<Type>())

#define CREATE_COMPONENT(TypeName, ...)\
ComponentFactory::Create(TypeName, __VA_ARGS__)

#define CREATE_EDITOR_COMPONENT(TypeName, ...)\
ComponentFactory::Create(TypeName, __VA_ARGS__)