#pragma once

class SavePointScript;
class PlayerScript;



class SavePointManager
{
public:
	// �̱���
	static SavePointManager& GetInstance();

	// ���̺� ����Ʈ ������ ����.
	void Reset();
	// ���̺�����Ʈ �߰�
	void AddSavePoint(SavePointScript* _savePoint);
	// ���̺� ����Ʈ�� �̵��Ѵ�.
	void GoBackLastSavePoint();
	// �ش� ���̺� ����Ʈ�� �̵��մϴ�.
	void GoBackSavePoint(SavePointScript* _savePoint);
	// null���̺� ����Ʈ�� �̵�
	void GoBackOriginPoint();
	// ������ ���̺� ����Ʈ�� �̵��� �� ����մϴ�.
	void JumpingSavePoint(INT _index);

private:
	// �̱���� 
	SavePointManager() = default; 
	~SavePointManager() = default;
	SavePointManager(const SavePointManager&) = delete; 
	SavePointManager& operator=(const SavePointManager&) = delete; 

	std::unordered_set<SavePointScript*> mSavePointArray;

};

