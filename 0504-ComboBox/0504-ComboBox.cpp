#include "Windows.h"
#include "Windowsx.h"
#include "resource.h"
#include <stdio.h>

const UINT nMax_Char = 100;

HWND g_hCombo;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ShowCurrentComboIndex(HWND hWnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_INITDIALOG:
	{
		g_hCombo = GetDlgItem(hWnd, IDC_COMBO1);
		ComboBox_AddString(g_hCombo, "a1");
		ShowCurrentComboIndex(hWnd);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

		// Combo Box : https://docs.microsoft.com/en-us/windows/desktop/controls/combo-boxes
		case IDC_COMBO1:
		{
			switch (HIWORD(wParam)) // ComboBox의 통지 메시지
			{
			case CBN_SELCHANGE: // 사용자가 콤보 리스트에서 아이템 선택
			{
				ShowCurrentComboIndex(hWnd);
			}
			break;
			//case CBN_EDITUPDATE: // CBN_EDITCHANGE와 다르게 변경된 텍스트가 화면에 출력되기 전에 전달됨
			case CBN_EDITCHANGE: // 콤보 박스의 텍스트 수정(선택 인덱스가 -1 됨)
			{
				ShowCurrentComboIndex(hWnd);
			}
			break;
			default:
				break;
			}
		}
		break;

		case IDC_BUTTON1: // add button
		{
			char str[nMax_Char] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, str, nMax_Char);
			ComboBox_AddString(g_hCombo, str); // 0번 인덱스에 추가

			ShowCurrentComboIndex(hWnd);
		}
		break;

		case IDC_BUTTON2: // insert button
		{
			char str[nMax_Char] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, str, nMax_Char);
			ComboBox_InsertString(
				g_hCombo,
				0, // 삽입할 인덱스, 아이템 개수보다 높은 인덱스이면 무시됨
				str
			);

			ShowCurrentComboIndex(hWnd);
		}
		break;

		case IDC_BUTTON3: // delete button
		{
			ComboBox_DeleteString(
				g_hCombo,
				0 // 삭제할 인덱스, 아이템 개수보다 높은 인덱스이면 무시됨
			);

			ShowCurrentComboIndex(hWnd);
		}
		break;

		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;

		default:
			break;
		}
	}
	break;

	default:
		break;
	}

	return 0;
}

void ShowCurrentComboIndex(HWND hWnd) {
	int selectedIndex = ComboBox_GetCurSel(g_hCombo);

	char str[nMax_Char] = {};
	sprintf_s(str, nMax_Char, "selected index : %d", selectedIndex);
	SetDlgItemText(hWnd, IDC_EDIT2, str);
}