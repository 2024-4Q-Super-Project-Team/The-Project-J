#include "pch.h"
#include "ScriptRegister.h"
#include "Component/ComponentFactory/ComponentFactory.h"

#include "Contents/GameApp/Script/CameraController.h"
#include "Contents/GameApp/Script/ModelSpawner.h"

void ScriptRegister::Register()
{
	REGISTER_COMPONENT(CameraController);
	REGISTER_COMPONENT(ModelSpawner);
}
