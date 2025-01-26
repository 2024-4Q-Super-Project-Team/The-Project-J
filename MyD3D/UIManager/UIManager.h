#pragma once

class Widget;

class UIManager
{
public:
    static BOOL Initialize();
    static void Finalization();
public:
    static void Update();
    static void Render();
public:
    // ��ο��� ���� ����
    static std::vector <Widget*> mDrawWidgetList;
    // ����Ʈ ��� �Լ�
    static void SetFocusViewport(ViewportScene* _pViewport);
    // Get �Լ�
    static SpriteBatch*         GetSpriteBatch()    { return pSpriteBatch; }
private:
    static SpriteBatch* pSpriteBatch;                // ��������Ʈ ��ġ

    static ViewportScene* mFocusViewport;            // �� ��Ʈ

    static Vector2  mFocusScreen;                    // ���� ��ũ�� ������
    static Vector2  mCurrScreen;                     // ���� ��ũ�� ������
    static Vector2  mScale;                          // ȭ�� ����

    // ȭ�� ���� ���� �Լ�
    static void SetScale() { mScale = mCurrScreen / mFocusScreen; }
};