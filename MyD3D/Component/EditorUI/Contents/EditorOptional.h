#pragma once

namespace Editor
{
    class Optional
    {
    public:
        // 줄 바꿈 여부를 설정
        inline void SetLineBreak(bool _bBreak) { mIsLineBreak = _bBreak; }
        // 사이 간격 설정
        inline void SetSpace(float _space) { mSpace = _space; }
        // 줄바꿈을 시도
        inline void TryLineBreak(){ if(!mIsLineBreak) ImGui::SameLine(mSpace); }
    protected:
        bool        mIsLineBreak    = true;   // 줄바꿈 여부
        float       mSpace          = 0;      // 사이 간격 (줄바꿈이 아닐 시만 사용)
    };
}