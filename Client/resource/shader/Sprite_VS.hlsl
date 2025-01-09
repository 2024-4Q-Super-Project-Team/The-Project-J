
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
    output.pos = float4(input.pos); // NDC 좌표 유지
    output.uv = input.uv; // 텍스처 좌표 전달
    return output;
}