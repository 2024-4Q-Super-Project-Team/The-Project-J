#pragma once

// �⺻ Desc, ���ؽ� ���� ��
class D3DGraphicsDefault
{
public:
	std::shared_ptr<D3DGraphicsDSV> gDepthStencilView;
	// �⺻ �ؽ��� ����
	static D3D11_TEXTURE2D_DESC DefaultTextureDesc;
	// �⺻ ���� �� ���� ����
	static D3D11_TEXTURE2D_DESC DefaultDSVTextureDesc;

public:
	static D3DGraphicsVertexBuffer* GetQuadFrameVertexBuffer();
	static D3DGraphicsIndexBuffer*	GetQuadFrameIndexBuffer();
private:
	// ��� ���ؽ����ۿ� �ε�������
	static  D3DGraphicsVertexBuffer* QuadFrameVertexBuffer;
	static  D3DGraphicsIndexBuffer*	 QuadFrameIndexBuffer;
};

class D3DGPUResourcePool
{
public:

};

struct QuadFrameVertex
{
	float Position[4]; // NDC ��ǥ
	float TexCoord[2]; // �ؽ�ó ��ǥ
};