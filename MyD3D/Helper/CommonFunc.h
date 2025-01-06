#pragma once

namespace Helper
{
    template <typename T>
    constexpr INT ToInt(T _val)
    {
        return static_cast<INT>(_val);
    }
    std::wstring ToWString(const std::string& _str);
    std::string ToString(const std::wstring& _wstr);
    // 키와 맵을 받아 find값을 포인터로 반환합니다.
	template <typename KEY, typename TYPE>
	TYPE* FindMap(const KEY& _key, std::unordered_map<KEY, TYPE>& _map)
	{
		auto itr = _map.find(_key);
        TYPE* result;
        if (itr != _map.end()) {
            result = &itr->second;
        }
        else result = nullptr;
        //TYPE* result = (itr != _map.end()) ? &itr->second : nullptr;
        return result;
	}
    // 리스트를 복사하면서 병합 : O(n)의 시간복잡도
    template <typename T>
    std::list<T> MergeLists(const std::list<T>& _target1, const std::list<T>& _target2)
    {
        std::vector<T> tempArr;

        tempArr.insert(tempArr.end(), _target1.cbegin(), _target1.cend());
        tempArr.insert(tempArr.end(), _target2.cbegin(), _target2.cend());

        return tempArr;
    }
    // 벡터를 복사하면서 병합 : O(n)의 시간복잡도
    template <typename T>
    std::vector<T> MergeVectors(const std::vector<T>& _target1, const std::vector<T>& _target2)
    {
        std::vector<T> tempArr;
        tempArr.reserve(_target1.size() + _target2.size()); // 벡터 크기 미리 예약 (최적화)

        tempArr.insert(tempArr.end(), _target1.begin(), _target1.end());
        tempArr.insert(tempArr.end(), _target2.begin(), _target2.end());

        return tempArr;
    }
    void DecomposeMatrix(const Matrix& mat, Vector3& position, Quaternion& rotation, Vector3& scale);
    HRESULT ReadFile(const WCHAR* filePath, std::vector<uint8_t>* data, std::size_t* size);
}