#pragma once
//
//class ResourceHandle
//{
//protected:
//	std::string	mMainKey = "";		// 리소스 매니저에서 쓸 키 값.
//	std::string	mSubKey = "";		// 메쉬의 경우, fbxModel내에서 사용하는 key값을 통해 불러와야 하므로, fbxModel내에서 사용하는 key값을 저장한다.
//	std::string	mPath = "";			// 리소스를 불러오기 위한 경로(FBX모델안에 있는 메쉬의 경우, 이 값은 fbx의 경로다.)
//public:
//	inline const std::string& GetKey() const { return mMainKey; }
//	inline const std::string& GetPath() const { return mPath; }
//	bool operator==(const ResourceHandle& other) const {
//		return mMainKey == other.mMainKey && mPath == other.mPath;
//	}
//};
//// 해시 함수
//namespace std {
//	template <>
//	struct hash<ResourceHandle> {
//		size_t operator()(const ResourceHandle& handle) const {
//			return hash<std::string>()(handle.GetKey()) ^ hash<std::string>()(handle.GetPath());
//		}
//	};
//}
