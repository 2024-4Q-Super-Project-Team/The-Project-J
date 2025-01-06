#pragma once

namespace Editor
{
    class Optional
    {
    public:
        // �� �ٲ� ���θ� ����
        inline void SetLineBreak(bool _bBreak) { mIsLineBreak = _bBreak; }
        // ���� ���� ����
        inline void SetSpace(float _space) { mSpace = _space; }
        // �ٹٲ��� �õ�
        inline void TryLineBreak(){ if(!mIsLineBreak) ImGui::SameLine(mSpace); }
    protected:
        bool        mIsLineBreak    = true;   // �ٹٲ� ����
        float       mSpace          = 0;      // ���� ���� (�ٹٲ��� �ƴ� �ø� ���)
    };
}