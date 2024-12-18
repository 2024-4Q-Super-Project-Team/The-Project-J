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
		UINT mPaletteID = 0;		// 모델 본 팔레트에서 자신이 속한 인덱스
		Matrix mOffsetMatrix;   // 해당 본으로 부터의 메쉬 옵셋
	public:
		const UINT GetPaletteID() { return mPaletteID; }
		const Matrix& GetBoneMatrix() { return mOffsetMatrix; }
	};

	// 본 파레트 등을 가지고 있는 클래스
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


