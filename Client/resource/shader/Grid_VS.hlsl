#include "Shared.fxh"

#define GridSpacing 10
GRID_VS_OUTPUT main(GRID_VS_INPUT input)
{
    GRID_VS_INPUT output;
   // ���� ��ǥ�迡�� ī�޶� �������� ����
    float3 worldPos = input.pos.xyz;

    // ī�޶� ��ġ�� ���� ����� �̵��� ���
    float3 relativeOffset = CameraPosition.xyz / GridSpacing;
    relativeOffset = float3(floor(relativeOffset.x), 0.0f, floor(relativeOffset.z)) * GridSpacing;

    // ���� ��ġ���� ������ �̵��� �߰� (ī�޶�� ���� �������� ����)
    worldPos += relativeOffset;

    // �׸��� ���� �������� �ݺ�
    worldPos.x = worldPos.x - fmod(worldPos.x, GridSpacing);
    worldPos.z = worldPos.z - fmod(worldPos.z, GridSpacing);

    // ���� ��ǥ -> �� -> �������� ��ȯ
    float4 finalWorldPos = float4(worldPos, 1.0f);
    output.pos = mul(finalWorldPos, CameraView);
    output.pos = mul(output.pos, CameraProjection);

    // ���� ���� ����
    output.color = input.color;
    return output;
}