#pragma once

// 불이 켜져 있는가, 옮기기 등에 대한 메소드와 변수 제공
// 추가로 불 메쉬에 대한 온/오프를 해줌.
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
	// [02/02 ~] 주형 작업 - 불 옮기기 시스템
	// 필요 변수 : 
	// 불 표현 메쉬
	// 불을 옮겨주고 있는 대상 오브젝트
	// 불에 타고 있는지 여부
	// 불 옮기기 작업이 진행 중인지 여부
	// 불 옮기기에 필요한 키 다운 틱
	// 불 옮기기 게이지 카운터
	////////////////////////////////////////////////
	Object* mBurnObject = nullptr;
	BurnObjectScript* mDstObject = nullptr;
	SerializeField(bool, isBurning, false);
	bool isProcessing = false;
	SerializeField(FLOAT, mFireMoveTick, 1.0f);
	FLOAT mFireMoveCount = 0.0f;
};

