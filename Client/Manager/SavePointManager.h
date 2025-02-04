#pragma once

class SavePointScript;


class SavePointManager
{
public:
	// 세이브 포인트 정보를 비운다.
	void Reset();
	//
	void AddSavePoint(SavePointScript* _savePoint);
	// 마지막 세이브 포인트로 이동한다.
	void GoBackSavePoint();
public:
	// 밀랍웅덩이 스크립트를 static 배열로 들고있는다.
	std::vector<SavePointScript*> mSavePointArray;

	// 고려할 사항
	// 1. 세이브포인트에 번호를 매겨서 번호순으로 정렬해 마지막 세이브 포인트를 찾는다.
};

