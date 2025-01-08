#pragma once

class Component;
class Object;


class ICreator
{
public:
	virtual ~ICreator() = default;
	virtual void* Create(void* args = nullptr) const = 0;
};

template <typename T, typename... Args>
class Creator : public ICreator {
public:
    void* Create(Object* owner, void* args = nullptr) const override {
        // args를 적절히 캐스팅하여 생성자에 전달
        if constexpr (sizeof...(Args) == 0) {
            return new T(); // 인자가 없는 경우
        }
        else {
            return CallConstructor(static_cast<std::tuple<Args...>*>(args));
        }
    }

private:
    template<typename Tuple>
    T* CallConstructor(Tuple* args) const
    {
        return std::apply([](Args... unpackedArgs) { return new T(unpackedArgs...); }, *args);
    }
};

class ComponentFactory
{
public:
    static void Register(const std::string& name, ICreator* creator)
    {
        mFactoryMap[name] = creator;
    }

    static void* Create(const std::string& name, Object* owner, void* args = nullptr)
    {
        auto it = mFactoryMap.find(name);
        if (it != mFactoryMap.end())
            return it->second->Create(args);
        return nullptr;
    }

private:
    static std::unordered_map<std::string, ICreator*> mFactoryMap;
};


#define REGISTER_COMPONENT(Type, OwnerPtr, ...)\
ComponentFactory::Register(#Type, new Creator<Type, OwnerPtr, __VA_ARGS__>())

#define CREATE_COMPONENT(TypeName, ...)\
ComponentFactory::Create(TypeName, __VA_ARGS__)

