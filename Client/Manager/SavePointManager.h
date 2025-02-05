#pragma once

class SavePointScript;


class SavePointManager
{
public:
	// 싱글톤
	static SavePointManager& GetInstance();

	// 세이브 포인트 정보를 비운다.
	void Reset();
	// 세이브포인트 추가
	void AddSavePoint(SavePointScript* _savePoint);
	// 마지막 세이브 포인트로 이동한다.
	void GoBackSavePoint();

private:
	// 싱글톤용 
	SavePointManager() = default; 
	~SavePointManager() = default;
	SavePointManager(const SavePointManager&) = delete; 
	SavePointManager& operator=(const SavePointManager&) = delete; 
	
	// 고려할 사항
	// 1. 세이브포인트에 번호를 매겨서 번호순으로 정렬해 마지막 세이브 포인트를 찾는다.
	int mCurrentIndex = 0;
	std::vector<SavePointScript*> mSavePointArray;

};

