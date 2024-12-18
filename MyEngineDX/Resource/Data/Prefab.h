#pragma once
#include "IResource.h"

namespace Resource
{
	class Prefab : public IResource
	{
	public:
		explicit Prefab();
		virtual ~Prefab();
	public:
		virtual const WCHAR* GetName() override;
	private:

	};
}