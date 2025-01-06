#pragma once

namespace Editor
{
    class EUID
    {
    public:
        explicit EUID(const char* _label);
        virtual ~EUID() = default;
    public:
        void ReSetUID(const char* _label);
    protected:
        std::string mUID;
    };
}
