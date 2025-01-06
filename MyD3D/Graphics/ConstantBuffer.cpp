#include "pch.h"
#include "ConstantBuffer.h"

BOOL MaterialCBuffer::GetUsingMap(eMaterialMapType _type)
{
    if ((UseMapFlag & (1 << (UINT)_type)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void MaterialCBuffer::SetUsingMap(eMaterialMapType _type, BOOL _bValue)
{
    if (_bValue == TRUE)
    {
        // _type에 해당하는 비트를 활성화
        UseMapFlag |= (1 << (UINT)_type);
    }
    else
    {
        // _type에 해당하는 비트를 비활성화
        UseMapFlag &= ~(1 << (UINT)_type);
    }
}

void MaterialCBuffer::ToggleUsingMap(eMaterialMapType _type)
{
    // _type에 해당하는 비트를 토글(켜져 있으면 끄고, 꺼져 있으면 켜기)
    UseMapFlag ^= (1 << (UINT)_type);
}
