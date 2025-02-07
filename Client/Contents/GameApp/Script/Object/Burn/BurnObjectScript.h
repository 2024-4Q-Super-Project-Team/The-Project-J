#pragma once

// ���� ���� �ִ°�, �ű�� � ���� �޼ҵ�� ���� ����
// �߰��� �� �޽��� ���� ��/������ ����.
class BurnObjectScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(BurnObjectScript);
public:
	virtual void Update() override;
public:
	inline void SetBurn(bool _isBurn) { isBurning.val = _isBurn; }
	inline void SetBurnObject(Object* _burnObject) { mBurnObject = _burnObject; }
	inline void SetBurnCount(FLOAT _val) { mFireMoveCount = _val; }
	inline void ProcessBurn(BurnObjectScript* _dst) { isProcessing = true; mDstObject = _dst; }
	inline void CancleProcess() { isProcessing = false; }
	inline bool IsBurning() { return isBurning.val; }
	inline bool IsProcessing() { return isProcessing; }
	inline auto GetDestObject() { return mDstObject; }
private:
	////////////////////////////////////////////////
	// [02/02 ~] ���� �۾� - �� �ű�� �ý���
	// �ʿ� ���� : 
	// �� ǥ�� �޽�
	// ���� �Ű��ְ� �ִ� ��� ������Ʈ
	// �ҿ� Ÿ�� �ִ��� ����
	// �� �ű�� �۾��� ���� ������ ����
	// �� �ű�⿡ �ʿ��� Ű �ٿ� ƽ
	// �� �ű�� ������ ī����
	////////////////////////////////////////////////
	Object* mBurnObject = nullptr;
	BurnObjectScript* mDstObject = nullptr;
	SerializeField(bool, isBurning, false);
	bool isProcessing = false;
	SerializeField(FLOAT, mFireMoveTick, 1.0f);
	FLOAT mFireMoveCount = 0.0f;
};

