#include "pch.h"
#include "CommonFunc.h"

namespace Helper
{
    void HRT(HRESULT _hr, const char* _errorMsg)
    {
        if (FAILED(_hr))
        {
            throw std::runtime_error(_errorMsg);
        }
    }
    std::wstring ToWString(const std::string& _str)
    {
        std::wstring wstr;
        wstr.assign(_str.begin(), _str.end());
        return wstr;
    }
    std::string ToString(const std::wstring& _wstr)
    {
        std::string str;
        str.assign(_wstr.begin(), _wstr.end());
        return str;
    }
    void DecomposeMatrix(const Matrix& mat, Vector3& position, Quaternion& rotation, Vector3& scale) {
        // Extract position
        position.x = mat.m[3][0];
        position.y = mat.m[3][1];
        position.z = mat.m[3][2];

        // Extract scale
        scale.x = std::sqrt(mat.m[0][0] * mat.m[0][0] + mat.m[0][1] * mat.m[0][1] + mat.m[0][2] * mat.m[0][2]);
        scale.y = std::sqrt(mat.m[1][0] * mat.m[1][0] + mat.m[1][1] * mat.m[1][1] + mat.m[1][2] * mat.m[1][2]);
        scale.z = std::sqrt(mat.m[2][0] * mat.m[2][0] + mat.m[2][1] * mat.m[2][1] + mat.m[2][2] * mat.m[2][2]);

        assert(scale.x != 0 && scale.y != 0 && scale.z != 0 && "Scale components must not be zero.");

        // Normalize the rotation matrix (Remove scale from rotation)
        Matrix rotationMatrix = mat;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (i == 0) {
                    rotationMatrix.m[i][j] /= scale.x; // X축 스케일
                }
                else if (i == 1) {
                    rotationMatrix.m[i][j] /= scale.y; // Y축 스케일
                }
                else if (i == 2) {
                    rotationMatrix.m[i][j] /= scale.z; // Z축 스케일
                }
            }
        }

        // Extract rotation (Quaternion)
        float trace = rotationMatrix.m[0][0] + rotationMatrix.m[1][1] + rotationMatrix.m[2][2];
        if (trace > 0) {
            float s = 0.5f / std::sqrt(trace + 1.0f);
            rotation.w = 0.25f / s;
            rotation.x = (rotationMatrix.m[1][2] - rotationMatrix.m[2][1]) * s;
            rotation.y = (rotationMatrix.m[2][0] - rotationMatrix.m[0][2]) * s;
            rotation.z = (rotationMatrix.m[0][1] - rotationMatrix.m[1][0]) * s;
        }
        else {
            if (rotationMatrix.m[0][0] > rotationMatrix.m[1][1] && rotationMatrix.m[0][0] > rotationMatrix.m[2][2]) {
                float s = 2.0f * std::sqrt(1.0f + rotationMatrix.m[0][0] - rotationMatrix.m[1][1] - rotationMatrix.m[2][2]);
                rotation.w = (rotationMatrix.m[1][2] - rotationMatrix.m[2][1]) / s;
                rotation.x = 0.25f * s;
                rotation.y = (rotationMatrix.m[0][1] + rotationMatrix.m[1][0]) / s;
                rotation.z = (rotationMatrix.m[0][2] + rotationMatrix.m[2][0]) / s;
            }
            else if (rotationMatrix.m[1][1] > rotationMatrix.m[2][2]) {
                float s = 2.0f * std::sqrt(1.0f + rotationMatrix.m[1][1] - rotationMatrix.m[0][0] - rotationMatrix.m[2][2]);
                rotation.w = (rotationMatrix.m[2][0] - rotationMatrix.m[0][2]) / s;
                rotation.x = (rotationMatrix.m[0][1] + rotationMatrix.m[1][0]) / s;
                rotation.y = 0.25f * s;
                rotation.z = (rotationMatrix.m[1][2] + rotationMatrix.m[2][1]) / s;
            }
            else {
                float s = 2.0f * std::sqrt(1.0f + rotationMatrix.m[2][2] - rotationMatrix.m[0][0] - rotationMatrix.m[1][1]);
                rotation.w = (rotationMatrix.m[0][1] - rotationMatrix.m[1][0]) / s;
                rotation.x = (rotationMatrix.m[0][2] + rotationMatrix.m[2][0]) / s;
                rotation.y = (rotationMatrix.m[1][2] + rotationMatrix.m[2][1]) / s;
                rotation.z = 0.25f * s;
            }
        }

        // Normalize the quaternion
        rotation.Normalize();
    }




    HRESULT ReadFile(const WCHAR* filePath, std::vector<uint8_t>* data, std::size_t* size)
    {
        std::wstring path = filePath;
        std::ifstream file(path.c_str(), std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            return E_INVALIDARG;
        }

        std::size_t filesize = static_cast<std::size_t>(file.tellg());
        // Return file size.
        *size = filesize;

        if (data == nullptr)
            return S_OK;

        // Return Data.
        std::vector<uint8_t> buffer(filesize);

        file.seekg(0);
        file.read((char*)buffer.data(), filesize);
        file.close();

        *data = std::move(buffer);

        return S_OK;
    }

    std::string to_utf8(std::wstring& wide_string)
    {
        {
            if (wide_string.empty()) {
                return "";
            }

            // UTF-8로 변환된 문자열의 길이를 계산합니다.
            int utf8_size = WideCharToMultiByte(
                CP_UTF8,                // 변환할 코드 페이지
                0,                      // 옵션 플래그
                wide_string.c_str(),    // 변환할 널 종료된 유니코드 문자열
                -1,                     // 문자열 길이 (널 종료까지 포함)
                nullptr,                // 출력 버퍼 (nullptr로 호출하여 크기만 계산)
                0,                      // 출력 버퍼 크기
                nullptr,                // 대체 문자 (사용하지 않음)
                nullptr                 // 대체 문자가 사용되었는지 여부
            );

            if (utf8_size <= 0) {
                throw std::runtime_error("Failed to convert wide string to UTF-8");
            }

            // UTF-8 문자열을 저장할 버퍼를 생성합니다.
            std::string utf8_string(utf8_size, 0);

            // 변환된 UTF-8 문자열을 버퍼에 씁니다.
            WideCharToMultiByte(
                CP_UTF8,                // 변환할 코드 페이지
                0,                      // 옵션 플래그
                wide_string.c_str(),    // 변환할 널 종료된 유니코드 문자열
                -1,                     // 문자열 길이 (널 종료까지 포함)
                &utf8_string[0],        // 출력 버퍼
                utf8_size,              // 출력 버퍼 크기
                nullptr,                // 대체 문자 (사용하지 않음)
                nullptr                 // 대체 문자가 사용되었는지 여부
            );

            // 버퍼에는 널 종료 문자가 포함되므로 제거합니다.
            utf8_string.pop_back();

            return utf8_string;
        }
    }
    std::wstring to_wstr(const std::string& utf8_string)
    {
        {
            if (utf8_string.empty()) {
                return L"";
            }

            int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, nullptr, 0);
            if (wide_size <= 0) {
                throw std::runtime_error("Failed to convert UTF-8 to wide string");
            }

            std::wstring wide_string(wide_size, 0);
            MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wide_string[0], wide_size);

            // Remove null terminator that was added by MultiByteToWideChar
            wide_string.pop_back();

            return wide_string;
        }
    }
}
