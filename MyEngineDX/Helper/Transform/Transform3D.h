#pragma once

class Transform3D
{
public:
	Transform3D(Transform3D* _parent = nullptr);
	virtual ~Transform3D();
public:
	void SetParent(Transform3D* _parent);
	void CalculateMatrix();
private:
	Transform3D* mParent;
	std::unordered_set<Transform3D*> mChildren;

	Vector3 mPostion;
	Vector3 mRotation;
	Vector3 mScale;

	Vector3 m_forward;
	Vector3 m_up;
	Vector3 m_right;

	Matrix mLocalMatrix; // »ó´ëÁÂÇ¥
	Matrix mWorldMatrix; // ¿ùµåÁÂÇ¥
public:
	const Vector3& GetWorldPosition();
	const Vector3& GetWorldRotation();
	const Vector3& GetWorldScale();

	const Matrix& GetlocalMatrix() { return mLocalMatrix; }
	const Matrix& GetWorldMatrix() { return mWorldMatrix; }

	const Vector3& Forward() { return m_forward; }
	const Vector3& Up() { return m_up; }
	const Vector3& Right() { return m_right; }

	Transform3D* GetParent() { return mParent; }
};