#include "pch.h"
#include "ScriptRegister.h"
#include "../MyD3D/Component/ComponentFactory/ComponentFactory.h"

#include "Contents/GameApp/Script/CameraController.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"
#include "Contents/GameApp/Script/Player/PlayerCollisionScript.h"
#include "Contents/GameApp/Script/Object/JumpPad/JumpPadScript.h"
#include "Contents/GameApp/Script/Object/Button/ButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/LowerPlatformButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/OnOffButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/SavePointScript.h"
#include "Contents/GameApp/Script/Object/Button/LowerWallButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/BoxDownButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/RaisePlatformButtonScript.h"
#include "Contents/GameApp/Script/Object/Button/RaiseGroundButtonScript.h"

#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Object/Box/BoxScript.h"
#include "Contents/GameApp/Script/Object/WoodWick/WoodWithWickScript.h"
#include "Contents/GameApp/Script/Object/Burn/MiniCandleScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurningWickScript.h"
#include "Contents/GameApp/Script/Object/HeavyRockSystem/HeavyRockSystem.h"
#include "Contents/GameApp/Script/Monster/MonsterScript.h"
#include "Contents/GameApp/Script/Monster/ScopeScript.h"
#include "Contents/GameApp/Script/Player/CheckIceSlope.h"
#include "Contents/GameApp/Script/SpriteAnimScript.h"
#include "Contents/GameApp/Script/GameProgressManager.h"
#include "Contents/GameApp/Script/Boss/newBossScript.h"
#include "Contents/GameApp/Script/Boss/newBossAttackScript.h"
#include "Contents/GameApp/Script/Player/BurnGageScript.h"

// Scene
#include "Contents/GameApp/Script/Scene/OpeningScript.h"
#include "Contents/GameApp/Script/Scene/TitleScript.h"
#include "Contents/GameApp/Script/Scene/EndingScript.h"

void ScriptRegister::Register()
{
	REGISTER_COMPONENT(CameraController);
	REGISTER_COMPONENT(PlayerScript);
	REGISTER_COMPONENT(PlayerCollisionScript);
	REGISTER_COMPONENT(JumpPadScript);
	REGISTER_COMPONENT(ButtonScript);
	REGISTER_COMPONENT(LowerPlatformButtonScript);
	REGISTER_COMPONENT(OnOffButtonScript);
	REGISTER_COMPONENT(LowerWallButtonScript);
	REGISTER_COMPONENT(RaiseGroundButtonScript);
	REGISTER_COMPONENT(RaisePlatformButtonScript);

	REGISTER_COMPONENT(BoxDownButtonScript);
	REGISTER_COMPONENT(BurnObjectScript);
	REGISTER_COMPONENT(MonsterScript);
	REGISTER_COMPONENT(ScopeScript);
	REGISTER_COMPONENT(SavePointScript);
	REGISTER_COMPONENT(CheckIceSlope);
	REGISTER_COMPONENT(BoxScript);
	REGISTER_COMPONENT(GameProgressManager);
	REGISTER_COMPONENT(newBossScript);
	REGISTER_COMPONENT(newBossAttackScript);
	REGISTER_COMPONENT(TitleScript);
	REGISTER_COMPONENT(EndingScript);
	REGISTER_COMPONENT(OpeningScript);
	REGISTER_COMPONENT(SpriteAnimScript);
	REGISTER_COMPONENT(BurnGageScript);
	REGISTER_COMPONENT(WoodWithWickScript);
	REGISTER_COMPONENT(MiniCandleScript);
	REGISTER_COMPONENT(BurningWickScript);
	REGISTER_COMPONENT(HeavyRockSystem);
}
