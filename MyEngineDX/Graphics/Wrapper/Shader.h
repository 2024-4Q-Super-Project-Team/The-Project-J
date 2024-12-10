#pragma once

namespace Graphics
{
	enum class eShaderStage
	{
		VS,		// Vertex Shader
		PS,		// Pixel Shader
	};

	class GraphicsDevice;

	class Shader
	{
	public:
		explicit Shader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
		virtual ~Shader();
	public:
		LPVOID __stdcall GetBufferPointer() { return mBlob->GetBufferPointer(); }
		SIZE_T __stdcall GetBufferSize() { return mBlob->GetBufferSize(); }
	protected:
		const WCHAR* mPath;
		ID3D10Blob* mBlob;
	};
	class VertexShader : public Shader
	{
	public:
		explicit VertexShader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
		virtual ~VertexShader();
	private:
		ID3D11VertexShader* mVertexShader;
	public:
		operator ID3D11VertexShader* () { return mVertexShader; }
	};
	class PixelShader : public Shader
	{
	public:
		explicit PixelShader(GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
		virtual ~PixelShader();
	public:
		ID3D11PixelShader* mPixelShader;
	public:
		operator ID3D11PixelShader* () { return mPixelShader; }
	};
	
}

