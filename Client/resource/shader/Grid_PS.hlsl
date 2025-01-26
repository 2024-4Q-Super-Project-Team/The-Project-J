#include "Shared.fxh"

float4 main(GRID_VS_OUTPUT output) : SV_TARGET
{
    return output.color;
}