#include "pch.h"
#include "Shader.h"
#include "../GraphicsDevice.h"

namespace Graphics
{
	Shader::Shader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel)
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* pErrorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, _entryPoint, _shaderModel,
			dwShaderFlags, 0, &mBlob, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				MessageBoxA(NULL, static_cast<char*>(pErrorBlob->GetBufferPointer()), "CompileShaderFromFile", MB_OK);
				pErrorBlob->Release();
			}
		}
	}
	Shader::~Shader()
	{
	}


	VertexShader::VertexShader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel)
		: Shader(_pDevice, _path, _entryPoint, _shaderModel)
	{
		HRESULT hr = _pDevice->GetDevice()->CreateVertexShader(
			mBlob->GetBufferPointer(),
			mBlob->GetBufferSize(),
			NULL,
			&mVertexShader);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, "FAILED", "CompileVertexShaderFromFile", MB_OK);
		}
	}
	VertexShader::~VertexShader()
	{
	}


	PixelShader::PixelShader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel)
		: Shader(_pDevice, _path, _entryPoint, _shaderModel)
	{
		HRESULT hr = _pDevice->GetDevice()->CreatePixelShader(
			mBlob->GetBufferPointer(),
			mBlob->GetBufferSize(),
			NULL,
			&mPixelShader);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, "FAILED", "CompilePixelShaderFromFile", MB_OK);
		}
	}
	PixelShader::~PixelShader()
	{
	}
}
