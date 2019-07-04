#include "Windows.h"
#include "resource.h"
#include "common.h"
#include "File.h"
#include <vector>
#include "Commctrl.h"
#include "StudentList.h"

HINSTANCE g_hInstance;
char g_strLoginId[Max_Account_Text];

StudentList g_studentList;
UINT g_nSelectedStudentIndex = -1;

INT_PTR CALLBACK LoginDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK LoginInfoChangeDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetLoginInfo(HWND hWnd);

void UpdateStudentsListView(HWND hWnd);
void UpdateDeleteButtonState(HWND hWnd, bool bEnable);

void SetWindowPositionToCenter(HWND hWnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;

	//LoginResultType loginResult = (LoginResultType)DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, LoginDlgProc);
	LoginResultType loginResult = LoginResultType::Success;
	if (loginResult == LoginResultType::Success) {
		log("login success!!");

		// 메인창 띄움
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), nullptr, MainDlgProc);
	}
	else {
		log("login Failed!!");
	}

	return 0;
}

// 메인 윈도우 프로시저
INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetLoginInfo(hWnd);
		UpdateDeleteButtonState(hWnd, g_nSelectedStudentIndex != -1);


		// 스크린 가운데 출력
		SetWindowPositionToCenter(hWnd);

		// 학생리스트 설정
		// 컬럼 추가
		HWND hListView = GetDlgItem(hWnd, IDC_LIST1);
		char colText0[] = "학번";
		char colText1[] = "이름";
		LVCOLUMN col = {};
		col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		col.fmt = LVCFMT_LEFT;
		col.cx = 100;
		col.pszText = colText0;
		ListView_InsertColumn(hListView, 0, &col); // 컬럼 추가0
		col.pszText = colText1;
		ListView_InsertColumn(hListView, 1, &col); // 컬럼 추가1

		// 리스트 아이템 전체가 선택되도록 설정
		ListView_SetExtendedListViewStyle(
			GetDlgItem(hWnd, IDC_LIST1),
			LVS_EX_FULLROWSELECT // 아이템 전체가 클릭되도록 한다.
			| LVS_EX_GRIDLINES // 서브아이템 사이에 그리드 라인을 넣는다.
		);

		// 파일에서 학생정보 읽기
		g_studentList.LoadStudents("students.txt");

		UpdateStudentsListView(hWnd);
	}
	break;

	case WM_NOTIFY:
	{
		switch (wParam)
		{
		case IDC_LIST1:
		{
			NMTTDISPINFO *nmttdispinfo = (NMTTDISPINFO*)lParam;
			if (nmttdispinfo->hdr.code == LVN_ITEMCHANGED) {
				g_nSelectedStudentIndex = ListView_GetNextItem(
					nmttdispinfo->hdr.hwndFrom, // 윈도우 핸들
					-1, // 검색을 시작할 인덱스
					LVNI_SELECTED // 검색 조건
				);
				if (g_nSelectedStudentIndex == -1) {
					// 
				}
				else {
					char id[Max_Account_Text];
					ListView_GetItemText(nmttdispinfo->hdr.hwndFrom, g_nSelectedStudentIndex, 0, id, Max_Account_Text);
					log(id);
				}

				UpdateDeleteButtonState(hWnd, g_nSelectedStudentIndex != -1);
			}
		}
		break;

		default:
			break;
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			File f("account.txt", "rt");
			char id[Max_Account_Text] = {};
			char pw[Max_Account_Text] = {};
			f.getLine(id, Max_Account_Text); // 한줄씩 읽는다.
			f.getLine(pw, Max_Account_Text); // 한줄씩 읽는다.

			char editId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editId);
			int resPw = strcmp(pw, editPw);

			LoginResultType loginResult = LoginResultType::Fail;
			if (resId == 0 && resPw == 0) {
				// 파일에 저장된 Id, Pw와 입력한 Id, Pw가 같으므로 로그인 성공
				loginResult = LoginResultType::Success;
			}

			EndDialog(hWnd, (INT_PTR)loginResult);
		}
		break;

		case IDC_BUTTON1: // 로그인 정보 변경 버튼
		{
			DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, LoginInfoChangeDlgProc);
		}
		break;

		case IDC_BUTTON2: // 학생 삭제 버튼
		{
			g_studentList.RemoveStudent(g_nSelectedStudentIndex);

			g_nSelectedStudentIndex = -1;
			UpdateStudentsListView(hWnd);
			UpdateDeleteButtonState(hWnd, g_nSelectedStudentIndex != -1);
		}
		break;

		case IDC_BUTTON4: // 학생 파일저장 버튼
		{
			g_studentList.SaveStudents();
		}
		break;

		case IDC_BUTTON3: // 학생 추가 버튼
		{
			LRESULT rId = SendMessage(GetDlgItem(hWnd, IDC_EDIT1), WM_GETTEXTLENGTH, 0, 0);
			LRESULT rName = SendMessage(GetDlgItem(hWnd, IDC_EDIT2), WM_GETTEXTLENGTH, 0, 0);

			if (rId == 0 || rName == 0) {
				MessageBox(hWnd, "input id or name", "input", MB_OK);
			}
			else {
				char id[Max_Account_Text];
				char name[Max_Account_Text];
				GetDlgItemText(hWnd, IDC_EDIT1, id, Max_Account_Text);
				GetDlgItemText(hWnd, IDC_EDIT2, name, Max_Account_Text);
				g_studentList.AddStudent(id, name);

				UpdateStudentsListView(hWnd);

				SetDlgItemText(hWnd, IDC_EDIT1, "");
				SetDlgItemText(hWnd, IDC_EDIT2, "");
			}
			
		}
		break;

		case IDCANCEL:
			EndDialog(hWnd, (INT_PTR)LoginResultType::Fail);
			break;

		default:
			break;

		}
		break;
	}

	default:
		break;
	}

	return false;
}

void CheckLogin() {

}

// 로그인 다이얼로그 프로시저
INT_PTR CALLBACK LoginDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowPositionToCenter(hWnd);
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			File f("account.txt", "rt");
			char id[Max_Account_Text] = {};
			char pw[Max_Account_Text] = {};
			f.getLine(id, Max_Account_Text); // 한줄씩 읽는다.
			f.getLine(pw, Max_Account_Text); // 한줄씩 읽는다.

			char editId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editId);
			int resPw = strcmp(pw, editPw);

			LoginResultType loginResult = LoginResultType::Fail;
			if (resId == 0 && resPw == 0) {
				// 파일에 저장된 Id, Pw와 입력한 Id, Pw가 같으므로 로그인 성공
				strcpy_s(g_strLoginId, id);
				loginResult = LoginResultType::Success;
				EndDialog(hWnd, (INT_PTR)loginResult);
			}
			else {
				// 로그인 실패
				MessageBox(hWnd, "로그인 실패!!", "로그인", MB_OK);
			}
		}
		break;

		case IDCANCEL:
			EndDialog(hWnd, (INT_PTR)LoginResultType::Fail);
			break;

		default:
			break;

		}
		break;
	}

	default:
		break;
	}

	return false;
}

// 로그인정보 수정 다이얼로그 프로시저
INT_PTR CALLBACK LoginInfoChangeDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hWnd, IDC_EDIT1));
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			File readAccountFile("account.txt", "rt");
			char id[Max_Account_Text] = {};
			char pw[Max_Account_Text] = {};
			readAccountFile.getLine(id, Max_Account_Text); // 한줄씩 읽는다.
			readAccountFile.getLine(pw, Max_Account_Text); // 한줄씩 읽는다.
			readAccountFile.close();

			char editCurId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editCurId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editCurId);
			int resPw = strcmp(pw, editPw);

			// 파일에 저장된 Id, Pw와 입력한 Id, Pw가 같다.
			if (resId == 0 && resPw == 0) {
				char editChangeId[Max_Account_Text] = {};
				char editChangePw[Max_Account_Text] = {};

				// 새로운 로그인 정보
				GetDlgItemText(hWnd, IDC_EDIT3, editChangeId, Max_Account_Text);
				GetDlgItemText(hWnd, IDC_EDIT4, editChangePw, Max_Account_Text);

				File writeAccountFile("account.txt", "wt");
				writeAccountFile.writeLine(editChangeId, Max_Account_Text);
				writeAccountFile.writeLine("\n", Max_Account_Text);
				writeAccountFile.writeLine(editChangePw, Max_Account_Text);
				writeAccountFile.close();

				EndDialog(hWnd, 0);
				MessageBox(hWnd, "성공!!", "로그인 정보 변경", MB_OK);
			}
		}
		break;
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return 0;
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

// 윈도우의 위치를 스크린 가운데로 옮김
void SetWindowPositionToCenter(HWND hWnd) {
	int screenX = GetSystemMetrics(SM_CXFULLSCREEN);
	int screenY = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rectClient;
	GetWindowRect(hWnd, &rectClient);
	UINT clientW = rectClient.right - rectClient.left;
	UINT clientH = rectClient.bottom - rectClient.top;

	MoveWindow(hWnd, (screenX / 2) - (clientW / 2), (screenY / 2) - (clientH / 2), clientW, clientH, false);
}

void SetLoginInfo(HWND hWnd) {
	SetDlgItemText(hWnd, IDC_EDIT4, g_strLoginId);
}

void UpdateStudentsListView(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LIST1);

	ListView_DeleteAllItems(hListView);

	// 아이템 추가
	LVITEM item = {};
	item.mask = LVIF_TEXT;
	item.iSubItem = 0; // 아이템을 처음 추가하므로 0번째 서브아이템을 선택한다.
	item.state;
	item.stateMask;
	int itemCount = 0;

	auto students = g_studentList.GetStudents();
	vector<Student>::iterator it = students->begin();
	while (it != students->end())
	{
		itemCount = ListView_GetItemCount(hListView);

		item.iItem = itemCount;
		item.pszText = it->Id;
		ListView_InsertItem(hListView, &item); // 아이템 추가0
		ListView_SetItemText(hListView, itemCount/*item idx*/, 1/*subitem idx*/, it->Name); // 서브아이템 추가0

		++it;
	}
}

void UpdateDeleteButtonState(HWND hWnd, bool bEnable) {
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON2), bEnable);
}