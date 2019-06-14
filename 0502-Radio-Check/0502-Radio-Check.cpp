#include "Windows.h"
#include "resource.h"
#include <stdio.h> // sprintf_s

const UINT nMax_Char = 100; // �ִ� ���ڿ� ����
const UINT nCheckCount = 2; // üũ ��ư ����
const UINT nRadioCount = 3; // ���� ��ư ����

const UINT arrCheckIds[nCheckCount] = { IDC_CHECK1, IDC_CHECK2 }; // üũ ��ư ���̵�
const UINT arrRadioIds[nRadioCount] = { IDC_RADIO1, IDC_RADIO2, IDC_RADIO3 }; // ���� ��ư ���̵�
const UINT arrEditIds[nCheckCount] = { IDC_EDIT1, IDC_EDIT2 }; // ����Ʈ ���̵�(���� ���ڿ� ��¿�)

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

UINT GetIdIndex(const UINT *ids, UINT cIds, UINT id); // ���̵�� ������ ��ȯ
void PrintCheckState(HWND hWnd, UINT nCheckId); // üũ ��ư�� ���¸� ���
void PrintRadioState(HWND hWnd); // ���� ��ư�� ���¸� ���

void ClearCheck(HWND hWnd); // ��� üũ ��ư�� ���� ����
void ClearRadio(HWND hWnd); // ��� ���� ��ư�� ���� ����

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// init check state
		for (size_t i = 0; i < nCheckCount; i++)
		{
			PrintCheckState(hWnd, i);
		}

		// init radio state
		PrintRadioState(hWnd);
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
			return true;
		}
		break;

		case IDC_RADIO1:
		case IDC_RADIO2:
		case IDC_RADIO3:
		{
			PrintRadioState(hWnd);
			return true;
		}
		break;

		case IDC_BUTTON1:
			ClearCheck(hWnd);
			break;

		case IDC_BUTTON2:
			ClearRadio(hWnd);
			break;

		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return true;
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

	UINT checked = IsDlgButtonChecked(hWnd, nCheckId); // ��ư ��Ʈ�ѷ� üũ ���� ������

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

	// ��Ʈ�� �ؽ�Ʈ ����
	SetDlgItemText(hWnd,
		nEditId, // ��Ʈ�� ID
		szCheckState // ������ �ؽ�Ʈ 
	);
}

void PrintRadioState(HWND hWnd) {
	for (size_t i = 0; i < nRadioCount; i++)
	{
		UINT nRadioId = arrRadioIds[i];
		UINT checked = IsDlgButtonChecked(hWnd, nRadioId); // ��ư ��Ʈ�ѷ� üũ ���� ������
		if (checked == BST_CHECKED) {
			char str[nMax_Char];

			sprintf_s(str, nMax_Char, "Radio%d checked", i + 1);
			SetDlgItemText(hWnd, IDC_EDIT3, str);
			return;
		}
	}

	SetDlgItemText(hWnd, IDC_EDIT3, "No checked radio");
}

UINT GetIdIndex(const UINT *ids, UINT cIds, UINT id) {
	for (size_t i = 0; i < cIds; i++)
	{
		if (ids[i] == id) {
			return i;
		}
	}
}

void ClearCheck(HWND hWnd) {
	for (size_t i = 0; i < nCheckCount; i++)
	{
		// ��ư ��Ʈ�ѷ� üũ ���� ����
		CheckDlgButton(hWnd, 
			arrCheckIds[i], // ��Ʈ�� ID
			BST_UNCHECKED // ����
		);
		PrintCheckState(hWnd, i);
	}
}

void ClearRadio(HWND hWnd) {
	// ��� 1.
	//for (size_t i = 0; i < nRadioCount; i++)
	//{
	//	CheckDlgButton(hWnd, arrRadioIds[i], BST_UNCHECKED); // ��ư ��Ʈ�ѷ� üũ ���� ����
	//}

	// ��� 2.
	CheckRadioButton(hWnd, 
		arrRadioIds[0], // ù ��° ���� ��ư ID
		arrRadioIds[nRadioCount - 1], // ������ ���� ��ư ID
		0 // ������ ���� ��ư ID
	);

	PrintRadioState(hWnd);
}