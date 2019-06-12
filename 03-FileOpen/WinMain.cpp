#include "Windows.h"
#include "resource.h"
#include <stdio.h> // sprintf_s()
#include <commdlg.h> // 없으면 GetOpenFileName() 호출 시 에러남

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
	g_hInstance = hInstance;

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

	return 0;
}

//char lpstrFile[MAX_PATH];
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowText(hWnd, "My Dialog");
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_OK:
		case ID_CANCEL:
			EndDialog(hWnd, 0);
			break;
		case IDC_OPEN:
		{
			// static 또는 전역변수로 선언하지 않으면 다이얼로그가 열리지 않음
			static char lpstrFile[MAX_PATH];
			// static 또는 전역변수로 선언하지 않아도 다이얼로그가 열림(강의에서는 필요하다고 함)
			char lpstrFileTitle[MAX_PATH];

			OPENFILENAME ofn = { 0, };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = "Every File(*.*)\0*.*\0";
			ofn.lpstrFile = lpstrFile;
			ofn.lpstrFileTitle = lpstrFileTitle;
			ofn.nMaxFile = MAX_PATH;
			ofn.nMaxFileTitle = MAX_PATH;
			ofn.lpstrTitle = "title";
			ofn.lpstrInitialDir = "C:\\";

			if (GetOpenFileName(&ofn)) {
				char szResult[MAX_PATH];
				sprintf_s(szResult, "Path: %s\n%s", ofn.lpstrFile, ofn.lpstrFileTitle);
				MessageBox(hWnd, szResult, NULL, MB_OK);
			};
		}
		break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}