#pragma once

namespace Resource
{
	class IResource
	{
	public:
		virtual const WCHAR* GetName() = 0;
	};
}


