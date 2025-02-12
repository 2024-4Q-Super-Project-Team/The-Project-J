#pragma once


/* 
���� ���� ����

-�����
�� ���� �÷��̾� �� �Ѹ��̶� ü���� 0�� �Ǹ� ���ӿ����� �ȴ�.

-��
�� ���� �÷��̾� ��� ���� ������ ���� �ʰ� �� ��� ���ӿ����� �ȴ�.

-����
�������� ������ �̵����� �� ���ӿ����� �ȴ�.
*/

enum class eGameProgressStatus
{
	PLAYING,	// �÷��� ��
	GAME_OVER,	// ���� ����
	PAUSE,		// �Ͻ� ����
};

enum eSceneType
{
	OPENING,
	TITLE,
	GAME,
	ENDING,
	CREDIT,
	STAGE_COUNT
};

class PlayerScript;
class newBossScript;

// �������� ���� ������ �����Ѵ�.
class GameProgressManager : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(GameProgressManager);
public:
	void Start();
	void Update();

	void ResetGame();			// ���� �����Ȳ�� ������ �ʱ�ȭ�Ѵ�. (���̺���� X) (ex. ���̺� ����Ʈ, ���� ���)
	void UpdateMap();			// ���� ���� ���� �ʿ� ���� ��Ȱ��ȭ ���ֱ� ����

	static void SetPlaying();
	static void UpdateGameOver();

public:
	static void SetPlayerInfo(PlayerScript* _player);
	static PlayerScript* GetPlayerInfo(INT _handle);
	static void SetBossInfo(newBossScript* _boss);
	static newBossScript* GetBossInfo();
	static void ChangeScene(eSceneType _sceneType);

	static eGameProgressStatus GetGameStatus();
private:
	static eGameProgressStatus		mGameStatus;
	static PlayerScript*			mPlayer[2];
	static newBossScript*			mBossScript;
	static Object*					mScenearray[STAGE_COUNT];	// �������� ������Ʈ �迭 (��Ȱ��ȭ ��Ʈ���� �ϱ� ����)
	static Object*					mMainCamera;
	static INT						mCurrentSceneNum;			// ���� ��������
};

