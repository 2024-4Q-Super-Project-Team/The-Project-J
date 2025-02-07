#include "pch.h"
#include "BossScript.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

#include "Manager/PlayerManager.h"
void BossScript::Start()
{
}

void BossScript::Update()
{
}

void BossScript::UpdateTransform()
{
	// 두 플레이어의 사이 값을 먼저 구한다.
	PlayerScript* Player1 = PlayerManager::GetPlayerInfo(0);
	PlayerScript* Player2 = PlayerManager::GetPlayerInfo(1);

	Vector3 Player1WorldPos = Player1->gameObject->transform->GetWorldPosition();
	Vector3 Player2WorldPos = Player2->gameObject->transform->GetWorldPosition();

	// 플레이어 간의 거리 벡터 계산
	Vector3 BetweenPos = Player1WorldPos - Player2WorldPos;


}

json BossScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["distance"] = mDistanceFromAxis.val;
	ret["min idle tick"] = mMinIdleTick.val;
	ret["max idle tick"] = mMaxIdleTick.val;

	return ret;
}

void BossScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	if (j.contains("distance"))
	{
		mDistanceFromAxis.val = j["distance"].get<FLOAT>();
	}
	if (j.contains("min idle tick"))
	{
		mMinIdleTick.val = j["min idle tick"].get<FLOAT>();
	}
	if (j.contains("max idle tick"))
	{
		mMaxIdleTick.val = j["max idle tick"].get<FLOAT>();
	}
}
