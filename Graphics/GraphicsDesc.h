#pragma once

#ifdef __d3d11_h__
typedef D3D11_INPUT_CLASSIFICATION 
#endif // D3D11
INPUT_CLASSIFICATION;

#ifdef __d3d11_h__
typedef DXGI_FORMAT
#endif // D3D11
FORMAT;


// ���ؽ����̴��� ����� �޾� ��ǲ���̾ƿ��� ������ �� �ʼ��� ���� ���� ��
struct InputLayoutDesc
{
    // �ø�ƽ �̸�
    LPCSTR					    SemanticName;			
    // �ø�ƽ ��
    UINT					    NumSemantics;			
    // ������ ����
    FORMAT		                Format;			
    // ���ؽ����� ����
    UINT					    InputSlot;		
    // ������(0xffffffff�� �ڵ� ������)
    UINT					    AlignedByteOffset = 0xffffffff;
    // �����Ͱ� ���ؽ� ���� �Ǵ� �ν��Ͻ� ����������
    INPUT_CLASSIFICATION        InputSlotClass;
    // �ν��Ͻ� ���� ���ؽ� ����
    UINT					    InstanceDataStepRate;	
};

#ifdef __d3d11_h__
typedef D3D11_FILTER
#endif // D3D11
SAMPLER_FILTER;

#ifdef __d3d11_h__
typedef D3D11_TEXTURE_ADDRESS_MODE
#endif // D3D11
SAMPLER_TEXTURE_ADDRESS_MODE;

#ifdef __d3d11_h__
typedef D3D11_COMPARISON_FUNC
#endif // D3D11
SMAPLER_COMPARISON_FUNC;

// ���÷� ���¸� �����ϱ� ���� �ʼ��� ����ϴ� ���� ��
struct SamplerDesc
{
    // �ؽ�ó ���͸� ���
    SAMPLER_FILTER                      Filter;
    // U(����)�� �ؽ�ó ��ǥ�� ���� 0~1 ���̸� ��� �� ó���� ���
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressU;
    // V(����)�� �ؽ�ó ��ǥ�� ���� 0~1 ���̸� ��� �� ó���� ���
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressV;
    // W(����)�� �ؽ�ó ��ǥ�� ���� 0~1 ���̸� ��� �� ó���� ���
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressW;
    //�ؽ�ó�� Mipmap ���ø� ������ ������ �ݴϴ�. ��� ���̸� �� ���� �ػ��� Mipmap�� ����ϰ�, ���� ���̸� �� ���� �ػ��� Mipmap�� ����մϴ�.
    FLOAT                               MipLODBias;
    // Filter ���� ANISOTROPIC�� ���� ���˴ϴ�. ���� Ŭ���� �ؽ�ó�� �񽺵��� �������� ���� ������ �� �� ���Դϴ�.
    UINT                                MaxAnisotropy;
    // �ؽ�ó ���� ���� ���ϴ� �� ���˴ϴ�. �ַ� �׸��� ���ΰ� ���� ���÷����� ���˴ϴ�.
    SMAPLER_COMPARISON_FUNC             ComparisonFunc;
    // �ؽ�ó ��ǥ�� ��� ������ ���� �� ����� ��� ������ �����մϴ�. AddressU/V/W�� BORDER�� ������ ��쿡�� ���˴ϴ�.
    FLOAT                               BorderColor[4];
    // Mipmap ���ø��� ����� �ּ� LOD(Mipmap Level of Detail)�� �����մϴ�. Mipmap ���ø��� �� �� �Ʒ��� �������� �ʵ��� �����մϴ�.
    FLOAT                               MinLOD = 0.0f;
    // Mipmap ���ø��� ����� �ִ� LOD�� �����մϴ�. Mipmap ���ø��� �� ���� �ʰ����� �ʵ��� �����մϴ�.
    FLOAT                               MaxLOD = D3D11_FLOAT32_MAX;
};

#ifdef __d3d11_h__
typedef D3D11_USAGE
#endif // D3D11
USAGE;


struct BufferDesc
{
    UINT    ByteWidth;
    USAGE   Usage;
    UINT    BindFlags;
    UINT    CPUAccessFlags;
    UINT    MiscFlags;
    UINT    StructureByteStride;
    // ���ؽ� ���� ����
    UINT    BufferStride;
    // ���� ������
    UINT    BufferOffset;
    // �ε������� �ڷ��� ����
    UINT    BufferFormat;
};

struct SubResourceData
{
    const void* pSysMem;
    UINT        SysMemPitch;
    UINT        SysMemSlicePitch;
};

struct ViewportDesc
{
    FLOAT TopLeftX;
    FLOAT TopLeftY;
    FLOAT Width;
    FLOAT Height;
    FLOAT MinDepth;
    FLOAT MaxDepth;
};