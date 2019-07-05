#include "Windows.h"
#include "resource.h"
#include "common.h"
#include "File.h"
#include <vector>
#include "Commctrl.h"
#include "StudentList.h"
#include "ScoreList.h"

HINSTANCE g_hInstance;

char g_strLoginId[Max_Account_Text];

// 학생
StudentList g_studentList;
UINT g_nSelectedStudentIndex = -1;
Student g_selectedStudent;
// 학기
UINT g_nSelectedSemesterIndex = -1;
int g_semesterRadioIds[Max_Semester] = { IDC_RADIO1 , IDC_RADIO2 , IDC_RADIO3 , IDC_RADIO4 };
// 점수
UINT g_nSelectedScoreIndex = -1;
ScoreList g_scoreList;

// 윈도우 프로시저
INT_PTR CALLBACK LoginDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK LoginInfoChangeDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void UpdateLoginInfo(HWND hWnd);
void UpdateStudentsListView(HWND hWnd);
void UpdateScoreListView(HWND hWnd);
void UpdateSelectedStudentInfo(HWND hWnd);

void UpdateStudentDeleteButtonState(HWND hWnd);
void UpdateScoreDeleteButtonState(HWND hWnd);

void SelectSemester(HWND hWnd, UINT index);
int GetSemesterIndexById(UINT id);

void SetWindowPositionToCenter(HWND hWnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;

	LoginResultType loginResult = (LoginResultType)DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, LoginDlgProc);
	//LoginResultType loginResult = LoginResultType::Success;
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
		UpdateLoginInfo(hWnd);
		UpdateStudentDeleteButtonState(hWnd);

		// 스크린 가운데 출력
		SetWindowPositionToCenter(hWnd);

		// 학생리스트 설정
		{
			// 컬럼 추가
			HWND hListView = GetDlgItem(hWnd, IDC_LIST1);
			char colText0[] = "학번";
			char colText1[] = "이름";
			LVCOLUMN col = {};
			col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			col.fmt = LVCFMT_LEFT;
			col.cx = 60;
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
		}


		// 점수리스트 설정
		{
			// 컬럼 추가
			HWND hListView = GetDlgItem(hWnd, IDC_LIST2);
			char colText0[] = "과목";
			char colText1[] = "점수";
			LVCOLUMN col = {};
			col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			col.fmt = LVCFMT_LEFT;
			col.cx = 60;
			col.pszText = colText0;
			ListView_InsertColumn(hListView, 0, &col); // 컬럼 추가0
			col.pszText = colText1;
			ListView_InsertColumn(hListView, 1, &col); // 컬럼 추가1

			// 리스트 아이템 전체가 선택되도록 설정
			ListView_SetExtendedListViewStyle(
				GetDlgItem(hWnd, IDC_LIST2),
				LVS_EX_FULLROWSELECT // 아이템 전체가 클릭되도록 한다.
				| LVS_EX_GRIDLINES // 서브아이템 사이에 그리드 라인을 넣는다.
			);
		}

		// 파일에서 학생정보 읽기
		g_studentList.LoadStudents("students.txt");
		UpdateStudentsListView(hWnd);
		// 1학기 선택
		SelectSemester(hWnd, 0);
		// 점수 리스트 갱신
		UpdateScoreListView(hWnd);
	}
	break;

	case WM_NOTIFY: // 공통 컨트롤러의 메시지 통지
	{
		switch (wParam)
		{
		case IDC_LIST1: // 학생 리스트
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
					ListView_GetItemText(nmttdispinfo->hdr.hwndFrom, g_nSelectedStudentIndex, 0, g_selectedStudent.Id, Max_Student_Id);
					ListView_GetItemText(nmttdispinfo->hdr.hwndFrom, g_nSelectedStudentIndex, 1, g_selectedStudent.Name, Max_Student_Name);
					log(g_selectedStudent.Id, g_selectedStudent.Name);
				}

				UpdateSelectedStudentInfo(hWnd);
				UpdateStudentDeleteButtonState(hWnd);

				SelectSemester(hWnd, 0);

				UpdateScoreListView(hWnd);
			}
		}
		break;

		case IDC_LIST2: // 성적 리스트
		{
			NMTTDISPINFO *nmttdispinfo = (NMTTDISPINFO*)lParam;
			if (nmttdispinfo->hdr.code == LVN_ITEMCHANGED) {
				g_nSelectedScoreIndex = ListView_GetNextItem(
					nmttdispinfo->hdr.hwndFrom, // 윈도우 핸들
					-1, // 검색을 시작할 인덱스
					LVNI_SELECTED // 검색 조건
				);
				if (g_nSelectedScoreIndex == -1) {
					// 
				}
				else {
					char course[Max_Account_Text];
					ListView_GetItemText(nmttdispinfo->hdr.hwndFrom, g_nSelectedScoreIndex, 0, course, Max_Account_Text);
					log(course);
				}

				UpdateScoreDeleteButtonState(hWnd);
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
			UpdateStudentDeleteButtonState(hWnd);
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

		case IDC_RADIO1: // 학기 라디오 버튼
		case IDC_RADIO2:
		case IDC_RADIO3:
		case IDC_RADIO4:
		{
			int radioIndex = GetSemesterIndexById(LOWORD(wParam));

			SelectSemester(hWnd, radioIndex);

			g_nSelectedScoreIndex = -1;
			UpdateScoreListView(hWnd);
			UpdateScoreDeleteButtonState(hWnd);
		}
		break;

		case IDC_BUTTON5: // 성적 추가 버튼
		{
			LRESULT rCourse = SendMessage(GetDlgItem(hWnd, IDC_EDIT6), WM_GETTEXTLENGTH, 0, 0);
			LRESULT rPoint = SendMessage(GetDlgItem(hWnd, IDC_EDIT7), WM_GETTEXTLENGTH, 0, 0);

			if (rCourse == 0 || rPoint == 0) {
				MessageBox(hWnd, "input course or point", "input", MB_OK);
			}
			else {
				char course[Max_Account_Text];
				char point[Max_Account_Text];
				GetDlgItemText(hWnd, IDC_EDIT6, course, Max_Account_Text);
				GetDlgItemText(hWnd, IDC_EDIT7, point, Max_Account_Text);

				Score score = { course, point };
				g_scoreList.AddScore(&score);

				UpdateScoreListView(hWnd);

				SetDlgItemText(hWnd, IDC_EDIT6, "");
				SetDlgItemText(hWnd, IDC_EDIT7, "");
			}

		}
		break;

		case IDC_BUTTON6: // 성적 삭제 버튼
		{
			g_scoreList.RemoveScore(g_nSelectedScoreIndex);

			g_nSelectedScoreIndex = -1;
			UpdateScoreListView(hWnd);
			UpdateScoreDeleteButtonState(hWnd);
		}
		break;

		case IDC_BUTTON7: // 성적 저장 버튼
		{
			// 학생 선택 확인
			if (g_nSelectedSemesterIndex == -1) {
				break;
			}

			// 학생 Id, 학기 정보로 현재 성적 리스트 파일로 저장
			g_scoreList.SaveScores(g_selectedStudent.Id, g_nSelectedSemesterIndex);
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

void UpdateLoginInfo(HWND hWnd) {
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

void UpdateStudentDeleteButtonState(HWND hWnd) {
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON2), g_nSelectedStudentIndex != -1);
}

void UpdateSelectedStudentInfo(HWND hWnd) {
	SetDlgItemText(hWnd, IDC_EDIT8, g_selectedStudent.Id);
	SetDlgItemText(hWnd, IDC_EDIT9, g_selectedStudent.Name);

	SelectSemester(hWnd, 0);
}

void UpdateScoreDeleteButtonState(HWND hWnd) {
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON6), g_nSelectedScoreIndex != -1);
}

void SelectSemester(HWND hWnd, UINT index)
{
	CheckDlgButton(hWnd,
		g_semesterRadioIds[index], // 컨트롤 ID
		BST_CHECKED // 상태
	);

	g_nSelectedSemesterIndex = index;

	// 학생과 학기가 선택됐다면 성적 파일을 읽어들임
	g_scoreList.LoadScore(g_selectedStudent.Id, g_nSelectedSemesterIndex);
}

int GetSemesterIndexById(UINT id)
{
	for (size_t i = 0; i < Max_Semester; i++)
	{
		if (g_semesterRadioIds[i] == id) {
			return i;
		}
	}
}

void UpdateScoreListView(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LIST2);

	ListView_DeleteAllItems(hListView);

	// 아이템 추가
	LVITEM item = {};
	item.mask = LVIF_TEXT;
	item.iSubItem = 0; // 아이템을 처음 추가하므로 0번째 서브아이템을 선택한다.
	item.state;
	item.stateMask;
	int itemCount = 0;

	auto scores = g_scoreList.GetScores();
	vector<Score>::iterator it = scores->begin();
	while (it != scores->end())
	{
		itemCount = ListView_GetItemCount(hListView);

		item.iItem = itemCount;
		item.pszText = it->Course;
		ListView_InsertItem(hListView, &item); // 아이템 추가0
		ListView_SetItemText(hListView, itemCount/*item idx*/, 1/*subitem idx*/, it->Point); // 서브아이템 추가0

		++it;
	}
}