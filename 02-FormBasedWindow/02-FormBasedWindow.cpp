#include "Windows.h"
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	return 0;
}

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
		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}