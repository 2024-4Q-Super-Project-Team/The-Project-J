#include "pch.h"
#include "ComponentFactory.h"

std::unordered_map<std::string_view, ICreator*> ComponentFactory::mFactoryMap;
std::vector<std::string_view> ComponentFactory::mComopnentNames;