#include "pch.h"
#include "SaveBase.h"
namespace Engine
{
	unsigned int SaveBase::mIdCount = 0;
	std::unordered_map<unsigned int, SaveBase*> SaveBase::mMap;
}