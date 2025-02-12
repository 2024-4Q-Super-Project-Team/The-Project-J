#pragma once

class SavePointScript;
class PlayerScript;



class SavePointManager
{
public:
	// 싱글톤
	static SavePointManager& GetInstance();

	// 세이브 포인트 정보를 비운다.
	void Reset();
	// 세이브포인트 추가
	void AddSavePoint(SavePointScript* _savePoint);
	// 세이브 포인트로 이동한다.
	void GoBackLastSavePoint();
	// 해당 세이브 포인트로 이동합니다.
	void GoBackSavePoint(SavePointScript* _savePoint);
	// null세이브 포인트로 이동
	void GoBackOriginPoint();
	// 강제로 세이브 포인트로 이동한 후 등록합니다.
	void JumpingSavePoint(INT _index);

private:
	// 싱글톤용 
	SavePointManager() = default; 
	~SavePointManager() = default;
	SavePointManager(const SavePointManager&) = delete; 
	SavePointManager& operator=(const SavePointManager&) = delete; 

	std::unordered_set<SavePointScript*> mSavePointArray;

};

