#pragma once

// 기본 Desc, 버텍스 버퍼 등
class D3DGraphicsDefault
{
public:
	std::shared_ptr<D3DGraphicsDSV> gDepthStencilView;
	// 기본 텍스쳐 포맷
	static D3D11_TEXTURE2D_DESC DefaultTextureDesc;
	// 기본 뎁스 뷰 생성 포맷
	static D3D11_TEXTURE2D_DESC DefaultDSVTextureDesc;

public:
	static D3DGraphicsVertexBuffer* GetQuadFrameVertexBuffer();
	static D3DGraphicsIndexBuffer*	GetQuadFrameIndexBuffer();
private:
	// 평면 버텍스버퍼와 인덱스버퍼
	static  D3DGraphicsVertexBuffer* QuadFrameVertexBuffer;
	static  D3DGraphicsIndexBuffer*	 QuadFrameIndexBuffer;
};

class D3DGPUResourcePool
{
public:

};

struct QuadFrameVertex
{
	float Position[4]; // NDC 좌표
	float TexCoord[2]; // 텍스처 좌표
};