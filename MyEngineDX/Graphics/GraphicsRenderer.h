#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class RenderTarget;
	class Shader;
	class VertexShader;
	class PixelShader;
	class InputLayout;
	class Buffer;
	class SamplerState;
	enum class eShaderStage;
	enum class eCBufferType;

	class Renderer
	{
	public:
		explicit Renderer(GraphicsDevice* _pDevice, ID3D11DeviceContext* _pContext);
		virtual ~Renderer();
		Renderer(Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = default;
		Renderer& operator=(Renderer&&) noexcept = default;
	public:
		void Release();
	public:
		BOOL SetRenderTarget(RenderTarget* _pRenderTarget);
	public:
		BOOL BeginRender();
		BOOL EndRender();
		BOOL DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex);
		
		
		/*BOOL SetVertexShader(VertexShader* _pVertexShader);
		BOOL SetPixelShader(PixelShader* _pPixelShader);*/
		BOOL BindInputLayout(InputLayout* _pInputLayout);
		BOOL BindShader(eShaderStage _stage, Shader* _pShader);
		BOOL BindConstantBuffer(eShaderStage _stage, eCBufferType _startSlot, Buffer* _constanceBuffer);
		BOOL BindSampler(eShaderStage _stage, UINT _startSlot, SamplerState* _sampler);
		
	private:
		ID3D11DeviceContext* mContext;
		RenderTarget* mCurRenderTarget;
		GraphicsDevice* mGraphicsDevice;
	public:
		RenderTarget* GetRenderTarget() { return mCurRenderTarget; }
	};
}
