#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "resource.h"

HINSTANCE g_hRootInstance;

TCHAR g_szClassName[] = TEXT("我是山里灵活的狗");
TCHAR g_szWindowName[] = TEXT("窗口");

void MessageProc_WM_COMMAND(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    switch (HIWORD(wParam)) {
    case 0:
        switch (LOWORD(wParam)) {
        case ID_CLOSS_ALL_WND:
            //关闭程序进程
            PostMessage(hWnd, WM_QUIT, 0, 0);
            break;
        default:
            break;
        }
        break;
    case 1:
        //空间通知处理
        break;
    default:
        return;
    }
}

void MessageProc_WM_CREATE(HWND hWnd) {

}

void GetMyNewWindow() {
    srand(GetTickCount64());
    RECT wndSize = { 0 };
    wndSize.right = 200;
    wndSize.bottom = 200;

    HWND hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, g_szClassName, g_szWindowName, WS_OVERLAPPEDWINDOW,
        /*CW_USEDEFAULT*/ rand() % (GetSystemMetrics(SM_CXSCREEN) - wndSize.right),
        /*CW_USEDEFAULT*/ rand() % (GetSystemMetrics(SM_CYSCREEN) - wndSize.bottom),
        wndSize.right, wndSize.bottom,    //窗口生成尺寸坐标 
        NULL, NULL, g_hRootInstance, NULL);;
    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOW);
    HMENU hMenu = LoadMenu(g_hRootInstance, MAKEINTRESOURCE(IDR_MENU1));
    SetMenu(hWnd, hMenu);
}

LRESULT CALLBACK WndMessageProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
    switch (msgID) {
    case WM_CLOSE:
        ////关闭
        //PostMessage(hWnd, WM_QUIT, 0, 0);
        //销毁之前的窗口 
        DestroyWindow(hWnd);
        //开新窗口,想玩的多开几个
        GetMyNewWindow();
        return 0;
    case WM_COMMAND:
        MessageProc_WM_COMMAND(hWnd, wParam, lParam);
        return 0;
    case WM_CREATE:
        MessageProc_WM_CREATE(hWnd);
        return 0;
    default:
        return DefWindowProc(hWnd, msgID, wParam, lParam);
    }
}

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
    g_hRootInstance = hInstance;

    WNDCLASSEX wcEx = { 0 };
    wcEx.cbSize = sizeof(wcEx);
    wcEx.hbrBackground = CreateSolidBrush(0xEFEFEF);
    wcEx.lpszClassName = g_szClassName;
    wcEx.hInstance = hInstance;
    wcEx.lpfnWndProc = WndMessageProc;
    wcEx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    RegisterClassEx(&wcEx);

    GetMyNewWindow();

    MSG msg = { 0 };
    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, PM_NOREMOVE, 0, 0))
        {
            //消息分发
            if (GetMessage(&msg, NULL, 0, 0) == FALSE) {
                //关闭按钮退出程序
                break;
            }
            else
            {
                //分发消息
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //其它处理
            continue;
        }
    }

    UnregisterClass(g_szClassName, hInstance);

    return 0;
}