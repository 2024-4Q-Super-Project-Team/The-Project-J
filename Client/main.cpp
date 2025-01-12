#include "pch.h"
#include <Windows.h>
#include "Application/GameApp.h"


// Ä¿¹Ô Å× - ½ºÅä 
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    GameApp App(hInstance);
    if (App.Initialize() == TRUE)
    {
        App.Run();
    }
    App.Finalization();
    return 0;
}