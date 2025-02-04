#include "Shared.fxh"

float4 main(OUTLINE_VS_OUTPUT output) : SV_TARGET
{
    return outlineColor;
}