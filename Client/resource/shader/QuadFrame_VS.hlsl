#include "Shared.fxh"

QUAD_VS_OUTPUT main(QUAD_VS_INPUT input)
{
    QUAD_VS_OUTPUT output;
    output.pos = input.pos;
    output.uv = input.uv;
    return output;
}