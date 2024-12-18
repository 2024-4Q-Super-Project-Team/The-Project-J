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
	// 위치 정보============================
	Vector3 mTrs[3];
	Matrix	mLocalMatrix; // 상대좌표
	Matrix	mWorldMatrix; // 월드좌표
	// ====================================
	//
	// 계층 정보===========================
	Transform* mRootParent;
	Transform* mParent;
	std::vector<Transform*> mChildren;
	//UINT mHierarchyLevel; // 계층 깊이
	// ====================================
	// 
	// 그 외===============================
	bool isDirty; // 더티플래그 (true면 재계산이 필요하다는 뜻)
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
	// lvalue 버전 (수정 가능, 더티 플래그 설정)
	Vector3& operator[](eTransformType _type)
	{
		isDirty = true; // 수정 가능 시 더티 플래그 설정
		return mTrs[_type];
	}

	// const rvalue 버전 (읽기 전용, 더티 플래그 설정 안 함)
	const Vector3& operator[](eTransformType _type) const
	{
		return mTrs[_type];
	}

};