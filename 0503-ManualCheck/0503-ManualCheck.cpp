#include "Windows.h"
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1: {
			// check 컨트롤의 클릭 통지 메시지
			// Button Messages : https://docs.microsoft.com/en-us/windows/desktop/controls/button-messages
			if (HIWORD(wParam) == BN_CLICKED) {
				// Check의 HWND 얻기
				HWND hCheck = GetDlgItem(hWnd, IDC_CHECK1);

				// check 컨트롤에 BM_GETCHECK 메시지 보내고 결과 받아서 현재 상태 알기
				LRESULT result = SendMessage(hCheck, BM_GETCHECK, 0, 0);
				if (result == BST_UNCHECKED) {
					// check 컨트롤이 비선택 상태라면 BM_SETCHECK 메시지를 보내 선택 상태로 만들기
					SendMessage(hCheck, BM_SETCHECK, BST_CHECKED, 0);
				}
				else {
					// check 컨트롤이 선택 상태라면 BM_SETCHECK 메시지를 비보내 선택 상태로 만들기
					SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				return true;
			}
		}

		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;

		default:
			break;
		}

	default:
		break;
	}

	return false;
}