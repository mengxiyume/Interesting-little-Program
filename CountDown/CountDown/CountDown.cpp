﻿// CountDown.cpp : 定义应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "CountDown.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
UINT g_uCountDown_s = 0;                        // 计时器剩余时间

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COUNTDOWN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COUNTDOWN));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COUNTDOWN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COUNTDOWN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 200, 100, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define IDT_TIMER_1SECOND 0xCCCCCCC1

////30分钟计时结束通知函数
//void CALLBACK Timerproc_30min(
//    HWND unnamedParam1,
//    UINT unnamedParam2,
//    UINT_PTR unnamedParam3,
//    DWORD unnamedParam4
//) {
//    MessageBox(NULL, TEXT("计时结束"), TEXT("提示"), MB_OK);
//}
////30分钟计时结束通知函数
//void CALLBACK Timerproc_1s(
//    HWND unnamedParam1,
//    UINT unnamedParam2,
//    UINT_PTR unnamedParam3,
//    DWORD unnamedParam4
//) {
//    --g_uCountDown_s;
//}
void NewTimer(HWND hWnd, UINT second) {
    g_uCountDown_s = second;
    //g_uCountDown_s = 10;       //测试用
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    SetTimer(hWnd, IDT_TIMER_1SECOND, (1000), NULL);
}

//设置计时器时间
void SetTime(HWND hWnd, UINT second) {
    if (g_uCountDown_s == 0) {
        NewTimer(hWnd, second);
        return;
    }
    if (second == 0)
    {
        KillTimer(hWnd, IDT_TIMER_1SECOND);
    }
    g_uCountDown_s = second;
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

//加时
void AppendTime(HWND hWnd, UINT second) {
    if (g_uCountDown_s == 0) {
        NewTimer(hWnd, second);
        return;
    }
    g_uCountDown_s += second;
    RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}

void Draw(HWND hWnd, HDC hdc) {
    RECT rect;
    RECT clientRect;
    //绘制一个剧中打印的时间
    GetClientRect(hWnd, &clientRect);
    rect.left = rect.top = 0;
    rect.right = clientRect.right - clientRect.left;
    rect.bottom = clientRect.bottom - clientRect.top;
    //时间获取
    TCHAR szBuffer[32];
    int min = g_uCountDown_s / 60, s = g_uCountDown_s % 60;
    _stprintf(szBuffer, TEXT("%02dm:%02ds"), min, s);
    DrawText(hdc, szBuffer, _tcslen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_TIMER_1SECOND:
            --g_uCountDown_s;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
            if (g_uCountDown_s == 0) {
                KillTimer(hWnd, IDT_TIMER_1SECOND);
                MessageBox(hWnd, TEXT("计时结束"), TEXT("提示"), MB_OK);
            }
            return 0;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_APPEND:
                //加时五分钟
                AppendTime(hWnd, 5 * 60);
                break;
            case IDM_NEW:
                //开启一个定时器任务 时间30分钟
                NewTimer(hWnd, 60 * 30);
                break;
            case IDM_RESETTIMER:
                SetTime(hWnd, 0);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            Draw(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}