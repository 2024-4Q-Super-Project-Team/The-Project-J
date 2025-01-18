#include "pch.h"
#include "ComponentFactory.h"

std::unordered_map<std::string_view, ICreator*> ComponentFactory::mFactoryMap;
std::vector<std::string_view> ComponentFactory::mComopnentNames;

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
	SAFE_DELETE_MAP (mFactoryMap);
}
