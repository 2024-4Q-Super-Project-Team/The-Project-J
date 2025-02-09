#pragma once


/* 
게임 오버 조건

-생명력
두 명의 플레이어 중 한명이라도 체력이 0이 되면 게임오버가 된다.

-빛
두 명의 플레이어 모두 빛을 가지고 있지 않게 될 경우 게임오버가 된다.

-낙사
스테이지 밖으로 이동했을 때 게임오버가 된다.
*/

enum class eGameProgressStatus
{
	PLAYING,	// 플레이 중
	GAME_OVER,	// 게임 오버
	PAUSE,		// 일시 정지
};

enum eStageType
{
	STAGE_01,
	STAGE_02,
	STAGE_03,
	STAGE_04,
	STAGE_COUNT,
};

class PlayerScript;

// 전반적인 게임 진행을 관리한다.
class GameProgressManager : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(GameProgressManager);
public:
	void Start();
	void Update();

	void ResetGame();			// 게임 진행상황을 완전히 초기화한다. (세이브부터 X) (ex. 세이브 포인트, 몬스터 등등)
	void UpdateMap();			// 현재 진행 중인 맵에 따라 비활성화 해주기 위한

	static void UpdateGameOver();
public:
	static void SetPlayerInfo(PlayerScript* _player);
	static PlayerScript* GetPlayerInfo(INT _handle);

	static eGameProgressStatus GetGameStatus();
private:
	static eGameProgressStatus		mGameStatus;
	static PlayerScript*			mPlayer[2];
	static Object*					mStagearray[STAGE_COUNT];	// 스테이지 오브젝트 배열 (비활성화 컨트롤을 하기 위함)
	static INT						mCurrentStageNum;			// 현재 스테이지
};

