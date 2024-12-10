#include "TestApp.h"
#include "EditorApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    TestApp App(hInstance);
#ifdef _DEBUG
    //EditorApp Editor(hInstance);
#endif // DEBUG
    bool InitApp = App.Initialize();
#ifdef _DEBUG
    bool InitEdit = true;
    //InitEdit = Editor.Initialize();
#endif // DEBUG

    if (InitApp)
    {
        App.Run();
#ifdef _DEBUG
        //if(InitEdit)
            //Editor.Run();
#endif // DEBUG
    }

    App.Finalization();

    return 0;
}