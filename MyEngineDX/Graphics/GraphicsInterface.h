#pragma once

namespace Graphics
{
	class IGraphicsResource
	{
	public:
		explicit IGraphicsResource(std::wstring_view _name) : mName(_name) {};
		virtual ~IGraphicsResource() {};
	public:
		virtual const std::wstring& GetName() { return mName; };
	protected:
		std::wstring mName;
	};
}

