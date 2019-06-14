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
			switch (HIWORD(wParam)) // ComboBox�� ���� �޽���
			{
			case CBN_SELCHANGE: // ����ڰ� �޺� ����Ʈ���� ������ ����
			{
				ShowCurrentComboIndex(hWnd);
			}
			break;
			//case CBN_EDITUPDATE: // CBN_EDITCHANGE�� �ٸ��� ����� �ؽ�Ʈ�� ȭ�鿡 ��µǱ� ���� ���޵�
			case CBN_EDITCHANGE: // �޺� �ڽ��� �ؽ�Ʈ ����(���� �ε����� -1 ��)
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
			ComboBox_AddString(g_hCombo, str); // 0�� �ε����� �߰�

			ShowCurrentComboIndex(hWnd);
		}
		break;

		case IDC_BUTTON2: // insert button
		{
			char str[nMax_Char] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, str, nMax_Char);
			ComboBox_InsertString(
				g_hCombo,
				0, // ������ �ε���, ������ �������� ���� �ε����̸� ���õ�
				str
			);

			ShowCurrentComboIndex(hWnd);
		}
		break;

		case IDC_BUTTON3: // delete button
		{
			ComboBox_DeleteString(
				g_hCombo,
				0 // ������ �ε���, ������ �������� ���� �ε����̸� ���õ�
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