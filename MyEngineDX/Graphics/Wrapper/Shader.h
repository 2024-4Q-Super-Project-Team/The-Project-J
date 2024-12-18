#pragma once

namespace Graphics
{
	enum class eShaderStage
	{
		VS,		// Vertex Shader
		PS,		// Pixel Shader
		ALL,    // All Type
	};

	class GraphicsDevice;

	class Shader : public IGraphicsResource
	{
	public:
		explicit Shader(std::wstring_view _name, GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
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
		explicit VertexShader(std::wstring_view _name, GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
		virtual ~VertexShader();
	private:
		ID3D11VertexShader* mVertexShader;
	public:
		operator ID3D11VertexShader* () { return mVertexShader; }
	};
	class PixelShader : public Shader
	{
	public:
		explicit PixelShader(std::wstring_view _name, GraphicsDevice* _pDevice, const WCHAR* _path, LPCSTR _entryPoint, LPCSTR _shaderModel);
		virtual ~PixelShader();
	public:
		ID3D11PixelShader* mPixelShader;
	public:
		operator ID3D11PixelShader* () { return mPixelShader; }
	};

}

