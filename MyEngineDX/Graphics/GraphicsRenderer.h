#pragma once

namespace Graphics
{
	class GraphicsDevice;
	class RenderTarget;

	class InputLayout;
	class SamplerState;

	class Shader;
	class VertexShader;
	class PixelShader;

	class Buffer;
	class ConstantBuffer;
	class VertexBuffer;
	class IndexBuffer;

	class Texture;

	enum class eShaderStage;

	using Batch = DirectX::PrimitiveBatch<DirectX::VertexPositionColor>;

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
		BOOL Clear();
		BOOL BeginRender();
		BOOL EndRender();
		BOOL DrawCall(UINT _numIndex, UINT _startIndex, INT _baseVertex);

		/*BOOL SetVertexShader(VertexShader* _pVertexShader);
		BOOL SetPixelShader(PixelShader* _pPixelShader);*/
		BOOL BindInputLayout(InputLayout* _pInputLayout);
		BOOL BindShader(eShaderStage _stage, Shader* _pShader);
		BOOL BindSampler(eShaderStage _stage, UINT _startSlot, SamplerState* _sampler);
		BOOL BindTextrue(eShaderStage _stage, UINT _startSlot, Graphics::Texture* _pTexture);

		BOOL BindConstantBuffer(eShaderStage _stage, UINT _startSlot, Buffer* _constanceBuffer);
		BOOL BindVertexBuffer(UINT _startSlot, UINT _numBuffers, VertexBuffer* _pVertexBuffers);
		BOOL BindIndexBuffer(IndexBuffer* _pIndexBuffer);
	private:
		ID3D11DeviceContext* mContext;
		RenderTarget* mCurRenderTarget;
		GraphicsDevice* mGraphicsDevice;
		Batch* mPrimitiveBatch;

	public:
		RenderTarget* GetRenderTarget() { return mCurRenderTarget; }
		Batch* GetPrimitiveBatch() { return mPrimitiveBatch; }
	};
}
