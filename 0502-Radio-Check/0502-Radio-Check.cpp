#include "Windows.h"
#include "resource.h"
#include <stdio.h> // sprintf_s

const UINT nMax_Char = 100; // 최대 문자열 길이
const UINT nCheckCount = 2; // 체크 버튼 개수
const UINT nRadioCount = 3; // 라디오 버튼 개수

const UINT arrCheckIds[nCheckCount] = { IDC_CHECK1, IDC_CHECK2 }; // 체크 버튼 아이디
const UINT arrRadioIds[nRadioCount] = { IDC_RADIO1, IDC_RADIO2, IDC_RADIO3 }; // 라디오 버튼 아이디
const UINT arrEditIds[nCheckCount] = { IDC_EDIT1, IDC_EDIT2 }; // 에디트 아이디(상태 문자열 출력용)

INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

UINT GetIdIndex(const UINT *ids, UINT cIds, UINT id); // 아이디로 순서를 반환
void PrintCheckState(HWND hWnd, UINT nCheckId); // 체크 버튼의 상태를 출력
void PrintRadioState(HWND hWnd); // 라디오 버튼의 상태를 출력

void ClearCheck(HWND hWnd); // 모든 체크 버튼의 선택 해제
void ClearRadio(HWND hWnd); // 모든 라디오 버튼의 선택 해제

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

	UINT checked = IsDlgButtonChecked(hWnd, nCheckId); // 버튼 컨트롤러 체크 상태 가져옴

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

	// 컨트롤 텍스트 변경
	SetDlgItemText(hWnd,
		nEditId, // 컨트롤 ID
		szCheckState // 변경할 텍스트 
	);
}

void PrintRadioState(HWND hWnd) {
	for (size_t i = 0; i < nRadioCount; i++)
	{
		UINT nRadioId = arrRadioIds[i];
		UINT checked = IsDlgButtonChecked(hWnd, nRadioId); // 버튼 컨트롤러 체크 상태 가져옴
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
		// 버튼 컨트롤러 체크 상태 변경
		CheckDlgButton(hWnd, 
			arrCheckIds[i], // 컨트롤 ID
			BST_UNCHECKED // 상태
		);
		PrintCheckState(hWnd, i);
	}
}

void ClearRadio(HWND hWnd) {
	// 방법 1.
	//for (size_t i = 0; i < nRadioCount; i++)
	//{
	//	CheckDlgButton(hWnd, arrRadioIds[i], BST_UNCHECKED); // 버튼 컨트롤러 체크 상태 변경
	//}

	// 방법 2.
	CheckRadioButton(hWnd, 
		arrRadioIds[0], // 첫 번째 라디오 버튼 ID
		arrRadioIds[nRadioCount - 1], // 마지막 라디오 버튼 ID
		0 // 선택할 라디오 버튼 ID
	);

	PrintRadioState(hWnd);
}