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
    // ��ο��� ���� ����
    static std::vector <UIWidget*> mDrawWidgetList;
    // ����Ʈ ��� �Լ�
    static void SetFocusViewport(ViewportScene* _pViewport);
    // Get �Լ�
    static SpriteBatch* GetSpriteBatch()    { return m_pSpriteBatch; }
private:
    static SpriteBatch* m_pSpriteBatch;                // ��������Ʈ ��ġ

    static ViewportScene* m_pFocusViewport;            // �� ��Ʈ

    static Vector2  mFocusScreen;                    // ���� ��ũ�� ������
    static Vector2  mCurrScreen;                     // ���� ��ũ�� ������
    static Vector2  mScale;                          // ȭ�� ����


    // ȭ�� ���� ���� �Լ�
    static void SetScale() { mScale = mCurrScreen / mFocusScreen; }
};