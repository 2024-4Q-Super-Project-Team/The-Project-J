#pragma once

class SavePointScript;


class SavePointManager
{
public:
	// �̱���
	static SavePointManager& GetInstance();

	// ���̺� ����Ʈ ������ ����.
	void Reset();
	// ���̺�����Ʈ �߰�
	void AddSavePoint(SavePointScript* _savePoint);
	// ������ ���̺� ����Ʈ�� �̵��Ѵ�.
	void GoBackSavePoint();

private:
	// �̱���� 
	SavePointManager() = default; 
	~SavePointManager() = default;
	SavePointManager(const SavePointManager&) = delete; 
	SavePointManager& operator=(const SavePointManager&) = delete; 
	
	// ����� ����
	// 1. ���̺�����Ʈ�� ��ȣ�� �Űܼ� ��ȣ������ ������ ������ ���̺� ����Ʈ�� ã�´�.
	int mCurrentIndex = 0;
	std::vector<SavePointScript*> mSavePointArray;

};

