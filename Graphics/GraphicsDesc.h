#pragma once

#ifdef __d3d11_h__
typedef D3D11_INPUT_CLASSIFICATION 
#endif // D3D11
INPUT_CLASSIFICATION;

#ifdef __d3d11_h__
typedef DXGI_FORMAT
#endif // D3D11
FORMAT;


// 버텍스셰이더의 양식을 받아 인풋레이아웃을 생성할 때 필수로 쓰는 설정 값
struct InputLayoutDesc
{
    // 시멘틱 이름
    LPCSTR					    SemanticName;			
    // 시멘틱 수
    UINT					    NumSemantics;			
    // 데이터 포맷
    FORMAT		                Format;			
    // 버텍스버퍼 슬롯
    UINT					    InputSlot;		
    // 오프셋(0xffffffff는 자동 오프셋)
    UINT					    AlignedByteOffset = 0xffffffff;
    // 데이터가 버텍스 버퍼 또는 인스턴스 데이터인지
    INPUT_CLASSIFICATION        InputSlotClass;
    // 인스턴스 단위 버텍스 개수
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

// 샘플러 상태를 설정하기 위해 필수로 사용하는 설정 값
struct SamplerDesc
{
    // 텍스처 필터링 방식
    SAMPLER_FILTER                      Filter;
    // U(수평)축 텍스처 좌표의 값이 0~1 사이를 벗어날 때 처리할 방식
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressU;
    // V(수직)축 텍스처 좌표의 값이 0~1 사이를 벗어날 때 처리할 방식
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressV;
    // W(깊이)축 텍스처 좌표의 값이 0~1 사이를 벗어날 때 처리할 방식
    SAMPLER_TEXTURE_ADDRESS_MODE        AddressW;
    //텍스처의 Mipmap 샘플링 레벨에 영향을 줍니다. 양수 값이면 더 낮은 해상도의 Mipmap을 사용하고, 음수 값이면 더 높은 해상도의 Mipmap을 사용합니다.
    FLOAT                               MipLODBias;
    // Filter 값이 ANISOTROPIC일 때만 사용됩니다. 값이 클수록 텍스처의 비스듬한 각도에서 세부 사항이 더 잘 보입니다.
    UINT                                MaxAnisotropy;
    // 텍스처 샘플 값을 비교하는 데 사용됩니다. 주로 그림자 맵핑과 같은 샘플러에서 사용됩니다.
    SMAPLER_COMPARISON_FUNC             ComparisonFunc;
    // 텍스처 좌표가 경계 밖으로 나갈 때 사용할 경계 색상을 설정합니다. AddressU/V/W가 BORDER로 설정된 경우에만 사용됩니다.
    FLOAT                               BorderColor[4];
    // Mipmap 샘플링에 사용할 최소 LOD(Mipmap Level of Detail)를 설정합니다. Mipmap 샘플링이 이 값 아래로 내려가지 않도록 제한합니다.
    FLOAT                               MinLOD = 0.0f;
    // Mipmap 샘플링에 사용할 최대 LOD를 설정합니다. Mipmap 샘플링이 이 값을 초과하지 않도록 제한합니다.
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
    // 버텍스 버퍼 정보
    UINT    BufferStride;
    // 버퍼 오프셋
    UINT    BufferOffset;
    // 인덱스버퍼 자료형 포맷
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