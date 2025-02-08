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

enum eStageType
{
	STAGE_01,
	STAGE_02,
	STAGE_03,
	STAGE_04,
	STAGE_COUNT,
};

class PlayerScript;

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
public:
	static void SetPlayerInfo(PlayerScript* _player);
	static PlayerScript* GetPlayerInfo(INT _handle);
private:
	static PlayerScript*	mPlayer[2];
	static Object*			mStagearray[STAGE_COUNT];	// �������� ������Ʈ �迭 (��Ȱ��ȭ ��Ʈ���� �ϱ� ����)
	static INT				mCurrentStageNum;			// ���� ��������
};

