#include "pch.h"
#include "Transform3D.h"

Transform3D::Transform3D(Transform3D* _parent)
	: mParent(_parent)
	, mChildren()
	, mPostion(), mRotation(), mScale()
	, mLocalMatrix(), mWorldMatrix()
{
}

Transform3D::~Transform3D()
{
	for (auto& child : mChildren)
	{
		child->mParent = nullptr;
	}
	// 객체의 삭제는 이 클래스를 소유하는 클래스의 역할로 전가
}

void Transform3D::CalculateMatrix()
{
	Matrix tr = DirectX::XMMatrixTranslation(mPostion.x, mPostion.y, mPostion.z);
	Matrix ro = DirectX::XMMatrixRotationX(mRotation.x)
		* DirectX::XMMatrixRotationY(mRotation.y)
		* DirectX::XMMatrixRotationZ(mRotation.z);
	Matrix sc = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	mLocalMatrix = sc * ro * tr;

	// 기저벡터 구하기
	m_up = Vector3::TransformNormal(Vector3::Up, ro);
	m_forward = Vector3::TransformNormal(Vector3::Forward, ro);
	m_right = Vector3::TransformNormal(Vector3::Right, ro);

	if (mParent)
	{
		mWorldMatrix = mLocalMatrix * mParent->mWorldMatrix;
	}
	else
	{
		mWorldMatrix = mLocalMatrix;
	}

	for (auto child : mChildren)
	{
		child->CalculateMatrix();
	}
}

void Transform3D::SetParent(Transform3D* _parent)
{
	// 부모가 이미 있으면 부모의 자식에서 자신을 제거
	if (nullptr != mParent)
	{
		auto find = mParent->mChildren.find(this);
		if (find != mParent->mChildren.end()) {
			mParent->mChildren.erase(find);
		}
	}

	// 새로운 부모가 있으면 자식 목록에 추가
	if (nullptr != _parent)
	{
		_parent->mChildren.insert(this);
	}

	// 부모를 설정
	mParent = _parent;
}
