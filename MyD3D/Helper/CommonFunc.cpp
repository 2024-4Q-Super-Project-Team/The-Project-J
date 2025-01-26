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
    std::string ToUTF8(const std::wstring& _wstr)
    {
        int utf8Size = WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string utf8String(utf8Size, 0);
        WideCharToMultiByte(CP_UTF8, 0, _wstr.c_str(), -1, &utf8String[0], utf8Size, nullptr, nullptr);
        return utf8String;
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
    HRESULT ABSPath_To_RelativePath(const std::wstring& _absPath, std::wstring& _relPath)
    {
        std::filesystem::path absPath;
        std::filesystem::path relativePath;
        try
        {
            // 절대 경로를 std::filesystem::path 객체로 변환
            absPath = _absPath;

            // 작업 디렉토리를 가져와 상대 경로 계산
            std::filesystem::path currentPath = std::filesystem::current_path();
            std::filesystem::path relativePath = std::filesystem::relative(absPath, currentPath);
            if (relativePath != L"")
            {
                _relPath = relativePath.wstring();
                return S_OK;
            }
            else
            {
                _relPath = L"";
                return S_FALSE;
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            // 에러 처리: 절대 경로를 상대 경로로 변환할 수 없는 경우
            Display::Console::Log(L"Failed to convert to relative path:");
            Display::Console::Log(_absPath, '\n');
            Display::Console::Log(e.what());
            return E_FAIL;
        }
    }
    HRESULT GetExtFromFilePath(const std::wstring& _filePath, std::wstring& _ext)
    {
        try
        {
            // std::filesystem::path를 이용하여 파일 경로를 처리
            std::filesystem::path filePath(_filePath);

            // 확장자를 가져옴
            if (filePath.has_extension()) {
                _ext = filePath.extension().wstring();
                return S_OK; // 성공적으로 확장자를 가져옴
            }
            else {
                _ext.clear(); // 확장자가 없는 경우 빈 문자열 반환
                return S_FALSE; // 확장자가 없음
            }
        }
        catch (const std::exception& e)
        {
            // 오류가 발생한 경우
            _ext.clear();
            return HRESULT_FROM_WIN32(ERROR_INVALID_DATA); // 일반적인 데이터 오류 코드 반환
        }
    }
    HRESULT GetFileNameFromFilePath(const std::wstring& _filePath, std::wstring& _fileName)
    {
        try {
            // std::filesystem::path를 이용하여 파일 경로를 처리
            std::filesystem::path filePath(_filePath);

            // 파일 이름을 가져옴
            if (filePath.has_filename()) {
                _fileName = filePath.filename().wstring();
                return S_OK; // 성공적으로 파일 이름을 가져옴
            }
            else {
                _fileName.clear(); // 파일 이름이 없는 경우 빈 문자열 반환
                return S_FALSE; // 파일 이름이 없음
            }
        }
        catch (const std::exception& e) {
            // 오류가 발생한 경우
            _fileName.clear();
            return HRESULT_FROM_WIN32(ERROR_INVALID_DATA); // 일반적인 데이터 오류 코드 반환
        }
    }
}
