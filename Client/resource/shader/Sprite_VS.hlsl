
struct VS_IN
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.pos = float4(input.pos); // NDC ��ǥ ����
    output.uv = input.uv; // �ؽ�ó ��ǥ ����
    return output;
}