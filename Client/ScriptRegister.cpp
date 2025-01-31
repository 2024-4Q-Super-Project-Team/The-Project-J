#include "pch.h"
#include "ScriptRegister.h"
#include "../MyD3D/Component/ComponentFactory/ComponentFactory.h"

#include "Contents/GameApp/Script/CameraController.h"
#include "Contents/GameApp/Script/ModelSpawner.h"
#include "Contents/GameApp/Script/PlayerScript.h"

void ScriptRegister::Register()
{
	REGISTER_COMPONENT(CameraController);
	REGISTER_COMPONENT(ModelSpawner);
	REGISTER_COMPONENT(PlayerScript);
}
