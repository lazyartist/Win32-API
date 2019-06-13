#include "Windows.h"
#include "resource.h"

#define Max_Char 100

const UINT nCheckCount = 2;
const UINT arrCheckIds[nCheckCount] = { IDC_CHECK1, IDC_CHECK2 };
const UINT arrEditIds[nCheckCount] = { IDC_EDIT1, IDC_EDIT2 };

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
void PrintCheckState(HWND hWnd, UINT nCheckId);
UINT GetIdIndex(const UINT *ids, UINT cIds, UINT id);

INT_PTR APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// 다이얼로그 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (size_t i = 0; i < nCheckCount; i++)
		{
			PrintCheckState(hWnd, i);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps = {};

		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_CHECK1:
		case IDC_CHECK2:
		{
			UINT id = LOWORD(wParam);
			UINT index = GetIdIndex(arrCheckIds, nCheckCount, id);
			PrintCheckState(hWnd, index);
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

void PrintCheckState(HWND hWnd, UINT index) {
	UINT nCheckId = arrCheckIds[index];
	UINT nEditId = arrEditIds[index];

	UINT checked = IsDlgButtonChecked(hWnd, nCheckId);

	const char *szCheckState = nullptr;
	switch (checked)
	{
	case BST_CHECKED:
		szCheckState = "Checked";
		break;
	case BST_UNCHECKED:
		szCheckState = "Unchecked";
		break;
	case BST_INDETERMINATE:
		szCheckState = "Indeterminate";
		break;
	default:
		break;
	}

	SetDlgItemText(hWnd, nEditId, szCheckState);
}

UINT GetIdIndex(const UINT *ids, UINT cIds, UINT id) {
	for (size_t i = 0; i < cIds; i++)
	{
		if (ids[i] == id) {
			return i;
		}
	}
}