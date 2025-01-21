#include "Shared.fxh"

[maxvertexcount(4)]
void main(point QUAD_VS_OUTPUT input[1], inout TriangleStream<QUAD_VS_OUTPUT> stream)
{
    // 1. 입력 데이터 가져오기
    float4 centerPos = input[0].pos; // 클립 공간 좌표
    //float particleSize = 6; // 스프라이트 크기 (클립 공간 단위)

    // 2. 사각형의 네 꼭짓점 위치 계산 (클립 공간 오프셋)
    float2 offsets[4] = {
        float2(-particleSize, particleSize),  // Top-left
        float2(particleSize, particleSize),   // Top-right
        float2(-particleSize, -particleSize), // Bottom-left
        float2(particleSize, -particleSize)   // Bottom-right
    };

    // 텍스처 좌표 (UV 매핑)
    float2 texCoords[4] = {
        float2(0.0, 0.0), // Top-left
        float2(1.0, 0.0), // Top-right
        float2(0.0, 1.0), // Bottom-left
        float2(1.0, 1.0)  // Bottom-right
    };

    // 3. 각 꼭짓점을 스트림에 추가
    for (int i = 0; i < 4; ++i)
    {
        QUAD_VS_OUTPUT output;
        output.pos = centerPos; // 중심 위치
        output.pos.xy += offsets[i]; // 클립 공간에서 오프셋 적용
        output.uv = texCoords[i];   // 텍스처 좌표 매핑

        stream.Append(output);
    }

    // 4. 스트립 종료
    stream.RestartStrip();
}