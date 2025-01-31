#include "pch.h"
#include "ScriptRegister.h"
#include "../MyD3D/Component/ComponentFactory/ComponentFactory.h"

#include "Contents/GameApp/Script/CameraController.h"
#include "Contents/GameApp/Script/ModelSpawner.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/JumpPadScript.h"
#include "Contents/GameApp/Script/Object/ButtonScript.h"

void ScriptRegister::Register()
{
	REGISTER_COMPONENT(CameraController);
	REGISTER_COMPONENT(ModelSpawner);
	REGISTER_COMPONENT(PlayerScript);
	REGISTER_COMPONENT(JumpPadScript);
	REGISTER_COMPONENT(ButtonScript);

}
