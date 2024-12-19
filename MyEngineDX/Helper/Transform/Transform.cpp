#include "pch.h"
#include "Transform.h"

Transform::Transform(void* _owner)
	: mOwnerAddr(reinterpret_cast<ULONG64>(_owner))
	, mRootParent(nullptr)
	, mParent(nullptr)
	, mChildren()
	, mLocalMatrix(Matrix::Identity)
	, mWorldMatrix(Matrix::Identity)
	, isDirty(false)
{
	mTrs[Position] = Vector3::Zero;
	mTrs[Rotation] = Vector3::Zero;
	mTrs[Scale] = Vector3::One;
}

Transform::~Transform()
{
	for (auto& child : mChildren)
	{
		child->mParent = nullptr;
	}
	// ��ü�� ������ �� Ŭ������ �����ϴ� Ŭ������ ���ҷ� ����
}

void Transform::UpdateMatrix()
{
	if (GetDirty())
	{
		Matrix tr = DirectX::XMMatrixTranslation(mTrs[Position].x, mTrs[Position].y, mTrs[Position].z);
		Matrix ro = DirectX::XMMatrixRotationX(mTrs[Rotation].x)
			* DirectX::XMMatrixRotationY(mTrs[Rotation].y)
			* DirectX::XMMatrixRotationZ(mTrs[Rotation].z);
		Matrix sc = DirectX::XMMatrixScaling(mTrs[Scale].x, mTrs[Scale].y, mTrs[Scale].z);

		mLocalMatrix = sc * ro * tr;
		mWorldMatrix = mParent ? mLocalMatrix * mParent->mWorldMatrix : mLocalMatrix;

		for (auto& child : mChildren)
		{
			child->UpdateMatrix();
		}
		isDirty = false;
	}
}
bool Transform::GetDirty()
{
	// �ڱ� �ڽ��� ��Ƽ�̰ų� �θ� �� �ϳ��� ��Ƽ�̸� true
	return isDirty || (mParent && mParent->GetDirty());
}
const Vector3 Transform::GetWorldPosition()
{
	UpdateMatrix();
	return Vector3(mWorldMatrix._41, mWorldMatrix._42, mWorldMatrix._43);
}
const Matrix& Transform::GetlocalMatrix()
{
	UpdateMatrix();
	return mLocalMatrix;
}
const Matrix& Transform::GetWorldMatrix()
{
	UpdateMatrix();
	return mWorldMatrix;
}
void Transform::SetParent(Transform* _parent)
{
	// �θ� �̹� ������ �θ��� �ڽĿ��� �ڽ��� ����
	if (mParent)
	{
		auto itr = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
		if(itr != mParent->mChildren.end())
			mParent->mChildren.erase(itr);
	}

	// ���ο� �θ� ������ �ڽ� ��Ͽ� �߰�
	if (_parent)
	{
		_parent->mChildren.push_back(this);
	}

	// �θ� ����
	mParent = _parent;
}
