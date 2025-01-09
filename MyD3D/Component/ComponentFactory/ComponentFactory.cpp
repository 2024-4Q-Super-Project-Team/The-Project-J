#include "pch.h"
#include "ComponentFactory.h"

std::unordered_map<std::string, ICreator*> ComponentFactory::mFactoryMap;
std::vector<std::string> ComponentFactory::mComopnentNames;