#include "Shared.fxh"

QUAD_VS_OUTPUT main(QUAD_VS_INPUT input)
{  
    QUAD_VS_OUTPUT output;
    output.pos = mul(input.pos, WorldMatrix);
    output.pos = mul(output.pos, ViewMatrix);
    output.pos = mul(output.pos, ProjectionMatrix);
    output.uv = input.uv;
    return output;
}