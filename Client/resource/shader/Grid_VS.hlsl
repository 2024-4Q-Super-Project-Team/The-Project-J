#include "Shared.fxh"

#define GridSpacing 10
GRID_VS_OUTPUT main(GRID_VS_INPUT input)
{
    GRID_VS_INPUT output;
   // 월드 좌표계에서 카메라 기준으로 보정
    float3 worldPos = input.pos.xyz;

    // 카메라 위치에 따른 상대적 이동량 계산
    float3 relativeOffset = CameraPosition.xyz / GridSpacing;
    relativeOffset = float3(floor(relativeOffset.x), 0.0f, floor(relativeOffset.z)) * GridSpacing;

    // 정점 위치에서 보정된 이동량 추가 (카메라와 같은 방향으로 보정)
    worldPos += relativeOffset;

    // 그리드 간격 기준으로 반복
    worldPos.x = worldPos.x - fmod(worldPos.x, GridSpacing);
    worldPos.z = worldPos.z - fmod(worldPos.z, GridSpacing);

    // 월드 좌표 -> 뷰 -> 프로젝션 변환
    float4 finalWorldPos = float4(worldPos, 1.0f);
    output.pos = mul(finalWorldPos, CameraView);
    output.pos = mul(output.pos, CameraProjection);

    // 정점 색상 전달
    output.color = input.color;
    return output;
}