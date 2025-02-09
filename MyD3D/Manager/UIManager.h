#pragma once

class UIWidget;

class UIManager
{
public:
    static BOOL Initialize();
    static void Finalization();
public:
    static void Update();
    static void Render();
    static void ClearDrawList();
public:
    // 드로우할 위젯 모음
    static std::vector <UIWidget*> mDrawWidgetList;
    // 뷰포트 얻는 함수
    static void SetFocusViewport(ViewportScene* _pViewport);
    // Get 함수
    static SpriteBatch* GetSpriteBatch()    { return m_pSpriteBatch; }
private:
    static SpriteBatch* m_pSpriteBatch;                // 스프라이트 배치

    static ViewportScene* m_pFocusViewport;            // 뷰 포트

    static Vector2  mFocusScreen;                    // 원래 스크린 사이즈
    static Vector2  mCurrScreen;                     // 현재 스크린 사이즈
    static Vector2  mScale;                          // 화면 비율


    // 화면 비율 설정 함수
    static void SetScale() { mScale = mCurrScreen / mFocusScreen; }
};