#define _CRT_SECURE_NO_WARNINGS
#include "AppendTimeWnd.h"
#include "CountDown.h"
#include <tchar.h>

LRESULT CALLBACK AppendTimeWindowProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	static HINSTANCE hInstance;
	static HWND hWndStaticMinute, hWndStaticSecond;
	static HWND hWndEditMinute, hWndEditSecond;
	static HWND hWndStaticTitle, hWndButtonOK;


	switch (msgID) 
	{
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		hWndStaticTitle = CreateWindowEx(0, TEXT("Static"), TEXT("׷��ʱ��"),
			WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 200, 20, hWnd, (HMENU)(-1), hInstance, NULL);
		hWndStaticMinute = CreateWindowEx(0, TEXT("Static"), TEXT("��:"),
			WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 20, 20, 20, hWnd, (HMENU)(-1), hInstance, NULL);
		hWndStaticSecond = CreateWindowEx(0, TEXT("Static"), TEXT("��:"),
			WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 40, 20, 20, hWnd, (HMENU)(-1), hInstance, NULL);
		//���ӵ�����	������λ��
		hWndEditMinute = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 20, 180, 20, hWnd, (HMENU)1000, hInstance, NULL);
		SendMessage(hWndEditMinute, EM_SETLIMITTEXT, 5, 0);
		//���ӵ����� ������λ��
		hWndEditSecond = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 40, 180, 20, hWnd, (HMENU)1001, hInstance, NULL);
		SendMessage(hWndEditSecond, EM_SETLIMITTEXT, 5, 0);
		//ȷ����ť
		hWndButtonOK = CreateWindowEx(0, TEXT("Button"), TEXT("ȷ��"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY | WS_TABSTOP,
			0, 60, 200, 20, hWnd, (HMENU)1002, hInstance, NULL);
		break;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			if (LOWORD(wParam) == 1002) {
				LPTSTR szBuffer = nullptr;
				UINT AppendCount = 0;
				size_t nLen = 0;

				//��ȡ������
				nLen = SendMessage(hWndEditMinute, EM_LINELENGTH, 0, 0);
				if (nLen > 0) {
					szBuffer = new TCHAR[nLen + 1];
					//��ȡ�ַ���
					ZeroMemory(szBuffer, (nLen + 1) * sizeof(TCHAR));
					szBuffer[0] = nLen + 1;
					SendMessage(hWndEditMinute, EM_GETLINE, 0, (LPARAM)szBuffer);
					//���������ַ���ת��Ϊ�ַ�
					int min = _tstoi(szBuffer);
					AppendCount += min * 60;
					delete[] szBuffer;
				}
				//��ȡ������
				nLen = SendMessage(hWndEditSecond, EM_LINELENGTH, 0, 0);
				if (nLen > 0) {
					szBuffer = new TCHAR[nLen + 1];
					//��ȡ�ַ���
					ZeroMemory(szBuffer, (nLen + 1) * sizeof(TCHAR));
					szBuffer[0] = nLen + 1;
					SendMessage(hWndEditSecond, EM_GETLINE, 0, (LPARAM)szBuffer);
					//���������ַ���ת��Ϊ�ַ�
					int sec = _tstoi(szBuffer);
					AppendCount += sec;
					delete[] szBuffer;
				}
				//��ʱ
				AppendTime(GetMainWindowHandle(), AppendCount);
			}
		}
		return 0;
	default:
		return DefWindowProc(hWnd, msgID, wParam, lParam);
	}
	return 0;
}

void RegisterAppendTimeWnd(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = AppendTimeWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COUNTDOWN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = APPEND_WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
}