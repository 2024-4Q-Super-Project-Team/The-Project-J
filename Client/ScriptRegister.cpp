#include "pch.h"
#include "ScriptRegister.h"
#include "../MyD3D/Component/ComponentFactory/ComponentFactory.h"

#include "Contents/GameApp/Script/CameraController.h"
#include "Contents/GameApp/Script/ModelSpawner.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Object/JumpPad/JumpPadScript.h"
#include "Contents/GameApp/Script/Object/Button/ButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/LowerPlatformButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/OnOffButtonScript.h"

#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Monster/ScopeScript.h"

void ScriptRegister::Register()
{
	REGISTER_COMPONENT(CameraController);
	REGISTER_COMPONENT(ModelSpawner);
	REGISTER_COMPONENT(PlayerScript);
	REGISTER_COMPONENT(JumpPadScript);
	REGISTER_COMPONENT(ButtonScript);
	REGISTER_COMPONENT(LowerPlatformButtonScript);
	REGISTER_COMPONENT(OnOffButtonScript);
	REGISTER_COMPONENT(BurnObjectScript);
	REGISTER_COMPONENT(MonsterScript);
	REGISTER_COMPONENT(ScopeScript);
}
