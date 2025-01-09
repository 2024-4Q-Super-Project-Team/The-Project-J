struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D inputTexture : register(t0); // SRV
SamplerState samplerState : register(s0);

float4 main(VS_OUT output) : SV_TARGET
{
    return inputTexture.Sample(samplerState, output.uv);
}