#pragma once

class SavePointScript;


class SavePointManager
{
public:
	// ���̺� ����Ʈ ������ ����.
	void Reset();
	//
	void AddSavePoint(SavePointScript* _savePoint);
	// ������ ���̺� ����Ʈ�� �̵��Ѵ�.
	void GoBackSavePoint();
public:
	// �ж������� ��ũ��Ʈ�� static �迭�� ����ִ´�.
	std::vector<SavePointScript*> mSavePointArray;

	// ����� ����
	// 1. ���̺�����Ʈ�� ��ȣ�� �Űܼ� ��ȣ������ ������ ������ ���̺� ����Ʈ�� ã�´�.
};

