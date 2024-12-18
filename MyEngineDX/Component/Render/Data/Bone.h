#pragma once
#define MAX_BONE_COUNT 100

namespace Graphics
{
	class BoneState;

	class BoneResource : public IGraphicsResource
	{
	public:
		explicit BoneResource(std::wstring_view _name, UINT _id, Matrix& _offsetMatrix);
		virtual ~BoneResource();
	private:
		UINT mPaletteID = 0;		// �� �� �ȷ�Ʈ���� �ڽ��� ���� �ε���
		Matrix mOffsetMatrix;   // �ش� ������ ������ �޽� �ɼ�
	public:
		const UINT GetPaletteID() { return mPaletteID; }
		const Matrix& GetBoneMatrix() { return mOffsetMatrix; }
	};

	// �� �ķ�Ʈ ���� ������ �ִ� Ŭ����
	class BoneState
	{
	public:
		explicit BoneState();
		virtual ~BoneState();
	public:
		void SetBoneMatrix(BoneResource* _pBoneResource, const Matrix& _worldMatrix);
	private:
		Matrix mBoneMatrixBuffer[MAX_BONE_COUNT];
	};
}


