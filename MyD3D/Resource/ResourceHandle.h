#pragma once
//
//class ResourceHandle
//{
//protected:
//	std::string	mMainKey = "";		// ���ҽ� �Ŵ������� �� Ű ��.
//	std::string	mSubKey = "";		// �޽��� ���, fbxModel������ ����ϴ� key���� ���� �ҷ��;� �ϹǷ�, fbxModel������ ����ϴ� key���� �����Ѵ�.
//	std::string	mPath = "";			// ���ҽ��� �ҷ����� ���� ���(FBX�𵨾ȿ� �ִ� �޽��� ���, �� ���� fbx�� ��δ�.)
//public:
//	inline const std::string& GetKey() const { return mMainKey; }
//	inline const std::string& GetPath() const { return mPath; }
//	bool operator==(const ResourceHandle& other) const {
//		return mMainKey == other.mMainKey && mPath == other.mPath;
//	}
//};
//// �ؽ� �Լ�
//namespace std {
//	template <>
//	struct hash<ResourceHandle> {
//		size_t operator()(const ResourceHandle& handle) const {
//			return hash<std::string>()(handle.GetKey()) ^ hash<std::string>()(handle.GetPath());
//		}
//	};
//}
