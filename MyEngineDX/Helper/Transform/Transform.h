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
	Transform();
	virtual ~Transform();
public:
	void UpdateMatrix();
	void SetParent(Transform* _parent);
private:
	// ��ġ ����============================
	Vector3 mTrs[3];
	Matrix	mLocalMatrix; // �����ǥ
	Matrix	mWorldMatrix; // ������ǥ
	// ====================================
	//
	// ���� ����===========================
	Transform* mRootParent;
	Transform* mParent;
	std::vector<Transform*> mChildren;
	//UINT mHierarchyLevel; // ���� ����
	// ====================================
	// 
	// �� ��===============================
	bool isDirty; // ��Ƽ�÷��� (true�� ������ �ʿ��ϴٴ� ��)
public:
	inline bool				GetDirty		();

	inline Transform*		GetParent		() { return mParent; }
	inline Transform*		GetChild		(UINT _index = 0) { return mChildren[0]; }
	inline const auto&		GetChildren		() { return mChildren; }

	inline const Vector3	GetWorldPosition();
	inline const Matrix&	GetlocalMatrix	();
	inline const Matrix&	GetWorldMatrix	();
	inline const Vector3&	Forward			() { return GetWorldMatrix().Forward(); }
	inline const Vector3&	Up				() { return GetWorldMatrix().Up(); }
	inline const Vector3&	Right			() { return GetWorldMatrix().Right(); }
	inline const Vector3&	Backward		() { return GetWorldMatrix().Backward(); }
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