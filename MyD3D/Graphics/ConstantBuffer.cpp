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
        // _type�� �ش��ϴ� ��Ʈ�� Ȱ��ȭ
        UseMapFlag |= (1 << (UINT)_type);
    }
    else
    {
        // _type�� �ش��ϴ� ��Ʈ�� ��Ȱ��ȭ
        UseMapFlag &= ~(1 << (UINT)_type);
    }
}

void MaterialCBuffer::ToggleUsingMap(eMaterialMapType _type)
{
    // _type�� �ش��ϴ� ��Ʈ�� ���(���� ������ ����, ���� ������ �ѱ�)
    UseMapFlag ^= (1 << (UINT)_type);
}
