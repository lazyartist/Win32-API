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

		// ����â ���
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), nullptr, MainDlgProc);
	}
	else {
		log("login Failed!!");
	}

	return 0;
}

// ���� ������ ���ν���
INT_PTR CALLBACK MainDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// ��ũ�� ��� ���
		SetWindowPositionToCenter(hWnd);

		// �л����� �б�
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
		students.reserve(studentsCount); // �޸� �̸� �Ҵ�

		for (size_t i = 0; i < studentsCount; i++)
		{
			fgets(studentInfoLine, Max_Student_Info_Line, file);

			token = strtok_s(studentInfoLine, "\t", &nextToken);
			strcpy_s(id, token);
			token = strtok_s(NULL, "\t", &nextToken);
			strcpy_s(name, token);

			// \n�� ����
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

		// �л����� ���

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
			f.getLine(id, Max_Account_Text); // ���پ� �д´�.
			f.getLine(pw, Max_Account_Text); // ���پ� �д´�.

			char editId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editId);
			int resPw = strcmp(pw, editPw);

			LoginResultType loginResult = LoginResultType::Fail;
			if (resId == 0 && resPw == 0) {
				// ���Ͽ� ����� Id, Pw�� �Է��� Id, Pw�� �����Ƿ� �α��� ����
				loginResult = LoginResultType::Success;
			}

			EndDialog(hWnd, (INT_PTR)loginResult);
		}
		break;

		case IDC_BUTTON1: // �α��� ���� ���� ��ư
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

// �α��� ���̾�α� ���ν���
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
			f.getLine(id, Max_Account_Text); // ���پ� �д´�.
			f.getLine(pw, Max_Account_Text); // ���پ� �д´�.

			char editId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editId);
			int resPw = strcmp(pw, editPw);

			LoginResultType loginResult = LoginResultType::Fail;
			if (resId == 0 && resPw == 0) {
				// ���Ͽ� ����� Id, Pw�� �Է��� Id, Pw�� �����Ƿ� �α��� ����
				loginResult = LoginResultType::Success;
				EndDialog(hWnd, (INT_PTR)loginResult);
			}
			else {
				// �α��� ����
				MessageBox(hWnd, "�α��� ����!!", "�α���", MB_OK);
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

// �α������� ���� ���̾�α� ���ν���
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
			readAccountFile.getLine(id, Max_Account_Text); // ���پ� �д´�.
			readAccountFile.getLine(pw, Max_Account_Text); // ���پ� �д´�.
			readAccountFile.close();

			char editCurId[Max_Account_Text] = {};
			char editPw[Max_Account_Text] = {};
			GetDlgItemText(hWnd, IDC_EDIT1, editCurId, Max_Account_Text);
			GetDlgItemText(hWnd, IDC_EDIT2, editPw, Max_Account_Text);

			int resId = strcmp(id, editCurId);
			int resPw = strcmp(pw, editPw);

			// ���Ͽ� ����� Id, Pw�� �Է��� Id, Pw�� ����.
			if (resId == 0 && resPw == 0) {
				char editChangeId[Max_Account_Text] = {};
				char editChangePw[Max_Account_Text] = {};

				// ���ο� �α��� ����
				GetDlgItemText(hWnd, IDC_EDIT3, editChangeId, Max_Account_Text);
				GetDlgItemText(hWnd, IDC_EDIT4, editChangePw, Max_Account_Text);

				File writeAccountFile("account.txt", "wt");
				writeAccountFile.writeLine(editChangeId, Max_Account_Text);
				writeAccountFile.writeLine("\n", Max_Account_Text);
				writeAccountFile.writeLine(editChangePw, Max_Account_Text);
				writeAccountFile.close();

				EndDialog(hWnd, 0);
				MessageBox(hWnd, "����!!", "�α��� ���� ����", MB_OK);
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

// �������� ��ġ�� ��ũ�� ����� �ű�
void SetWindowPositionToCenter(HWND hWnd) {
	int screenX = GetSystemMetrics(SM_CXFULLSCREEN);
	int screenY = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rectClient;
	GetWindowRect(hWnd, &rectClient);
	UINT clientW = rectClient.right - rectClient.left;
	UINT clientH = rectClient.bottom - rectClient.top;

	MoveWindow(hWnd, (screenX / 2) - (clientW / 2), (screenY / 2) - (clientH / 2), clientW, clientH, false);
}