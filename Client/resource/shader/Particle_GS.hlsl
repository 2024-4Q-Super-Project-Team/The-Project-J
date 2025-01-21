#include "Shared.fxh"

[maxvertexcount(4)]
void main(point QUAD_VS_OUTPUT input[1], inout TriangleStream<QUAD_VS_OUTPUT> stream)
{
    // 1. �Է� ������ ��������
    float4 centerPos = input[0].pos; // Ŭ�� ���� ��ǥ
    //float particleSize = 6; // ��������Ʈ ũ�� (Ŭ�� ���� ����)

    // 2. �簢���� �� ������ ��ġ ��� (Ŭ�� ���� ������)
    float2 offsets[4] = {
        float2(-particleSize, particleSize),  // Top-left
        float2(particleSize, particleSize),   // Top-right
        float2(-particleSize, -particleSize), // Bottom-left
        float2(particleSize, -particleSize)   // Bottom-right
    };

    // �ؽ�ó ��ǥ (UV ����)
    float2 texCoords[4] = {
        float2(0.0, 0.0), // Top-left
        float2(1.0, 0.0), // Top-right
        float2(0.0, 1.0), // Bottom-left
        float2(1.0, 1.0)  // Bottom-right
    };

    // 3. �� �������� ��Ʈ���� �߰�
    for (int i = 0; i < 4; ++i)
    {
        QUAD_VS_OUTPUT output;
        output.pos = centerPos; // �߽� ��ġ
        output.pos.xy += offsets[i]; // Ŭ�� �������� ������ ����
        output.uv = texCoords[i];   // �ؽ�ó ��ǥ ����

        stream.Append(output);
    }

    // 4. ��Ʈ�� ����
    stream.RestartStrip();
}