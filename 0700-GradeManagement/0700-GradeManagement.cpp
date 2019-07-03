#include "Windows.h"
#include "resource.h"
#include "common.h"
#include "File.h"
//#include <vector>

HINSTANCE g_hInstance;

INT_PTR CALLBACK LoginDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK LoginInfoChangeDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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
		// 스크린 가운데 출력
		SetWindowPositionToCenter(hWnd);

		// 학생정보 읽기
		FILE *file;
		fopen_s(&file, "students.txt", "rt");
		char studentsFileInfo[Max_Student_File_Info] = {};
		fgets(studentsFileInfo, Max_Student_Info_Line, file);
		int studentsCount = atoi(studentsFileInfo);

		char studentInfoLine[Max_Student_Info_Line] = {};
		char id[Max_Student_Id];
		char name[Max_Student_Name];
		char *token;
		char *nextToken;

		vector<Student> students;
		students.reserve(studentsCount); // 메모리 미리 할당

		for (size_t i = 0; i < studentsCount; i++)
		{
			fgets(studentInfoLine, Max_Student_Info_Line, file);

			token = strtok_s(studentInfoLine, "\t", &nextToken);
			strcpy_s(id, token);
			token = strtok_s(NULL, "\t", &nextToken);
			strcpy_s(name, token);

			// \n을 제거
			id[strcspn(id, "\n")] = 0;
			name[strcspn(name, "\n")] = 0;

			Student student = {};
			strcpy_s(student.Id, id);
			strcpy_s(student.Name, name);

			students.push_back(student);
		}

		vector<Student>::iterator it = students.begin();
		while (it != students.end())
		{
			log(it->Id);
			log(it->Name);
			++it;

			log("---");
		}

		// 학생정보 출력

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