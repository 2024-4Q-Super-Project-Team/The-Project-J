#pragma once

enum eTransformType
{
	Position,
	Rotation,
	Scale,
};

class Transform
{
public:
	Transform(void* _owner);
	virtual ~Transform();
public:
	void UpdateMatrix();
	void SetParent(Transform* _parent);
private:
	// ���� ��ü�� ���� �ּҰ�(Ÿ��ĳ���� �ʿ�)
	ULONG64			mOwnerAddr;
	// ��ġ ����============================
	Vector3			mTrs[3];
	Matrix			mLocalMatrix; // �����ǥ
	Matrix			mWorldMatrix; // ������ǥ
	// ====================================
	//
	// ���� ����===========================
	Transform*		mRootParent;
	Transform*		mParent;
	std::vector<Transform*> mChildren;
	//UINT mHierarchyLevel; // ���� ����
	// ====================================
	// 
	// �� ��===============================
	bool isDirty; // ��Ƽ�÷��� (true�� ������ �ʿ��ϴٴ� ��)
public:
	template <typename Type>
	Type*			GetOwner() const { return reinterpret_cast<Type*>(mOwnerAddr); }

	bool				GetDirty		();

	Transform*		GetParent		() { return mParent; }
	Transform*		GetChild		(UINT _index = 0) { return mChildren[0]; }
	const auto&		GetChildren		() { return mChildren; }

	const Vector3	GetWorldPosition();
	const Matrix&	GetlocalMatrix	();
	const Matrix&	GetWorldMatrix	();
	const Vector3&	Forward			() { return GetWorldMatrix().Forward(); }
	const Vector3&	Up				() { return GetWorldMatrix().Up(); }
	const Vector3&	Right			() { return GetWorldMatrix().Right(); }
	const Vector3&	Backward		() { return GetWorldMatrix().Backward(); }
public:
	// lvalue ���� (���� ����, ��Ƽ �÷��� ����)
	Vector3& operator[](eTransformType _type)
	{
		isDirty = true; // ���� ���� �� ��Ƽ �÷��� ����
		return mTrs[_type];
	}

	// const rvalue ���� (�б� ����, ��Ƽ �÷��� ���� �� ��)
	const Vector3& operator[](eTransformType _type) const
	{
		return mTrs[_type];
	}

};