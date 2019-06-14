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
			// check ��Ʈ���� Ŭ�� ���� �޽���
			// Button Messages : https://docs.microsoft.com/en-us/windows/desktop/controls/button-messages
			if (HIWORD(wParam) == BN_CLICKED) {
				// Check�� HWND ���
				HWND hCheck = GetDlgItem(hWnd, IDC_CHECK1);

				// check ��Ʈ�ѿ� BM_GETCHECK �޽��� ������ ��� �޾Ƽ� ���� ���� �˱�
				LRESULT result = SendMessage(hCheck, BM_GETCHECK, 0, 0);
				if (result == BST_UNCHECKED) {
					// check ��Ʈ���� ���� ���¶�� BM_SETCHECK �޽����� ���� ���� ���·� �����
					SendMessage(hCheck, BM_SETCHECK, BST_CHECKED, 0);
				}
				else {
					// check ��Ʈ���� ���� ���¶�� BM_SETCHECK �޽����� �񺸳� ���� ���·� �����
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