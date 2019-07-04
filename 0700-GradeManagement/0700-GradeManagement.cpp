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
		SetLoginInfo(hWnd);
		UpdateDeleteButtonState(hWnd, g_nSelectedStudentIndex != -1);


		// ��ũ�� ��� ���
		SetWindowPositionToCenter(hWnd);

		// �л�����Ʈ ����
		// �÷� �߰�
		HWND hListView = GetDlgItem(hWnd, IDC_LIST1);
		char colText0[] = "�й�";
		char colText1[] = "�̸�";
		LVCOLUMN col = {};
		col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		col.fmt = LVCFMT_LEFT;
		col.cx = 100;
		col.pszText = colText0;
		ListView_InsertColumn(hListView, 0, &col); // �÷� �߰�0
		col.pszText = colText1;
		ListView_InsertColumn(hListView, 1, &col); // �÷� �߰�1

		// ����Ʈ ������ ��ü�� ���õǵ��� ����
		ListView_SetExtendedListViewStyle(
			GetDlgItem(hWnd, IDC_LIST1),
			LVS_EX_FULLROWSELECT // ������ ��ü�� Ŭ���ǵ��� �Ѵ�.
			| LVS_EX_GRIDLINES // ��������� ���̿� �׸��� ������ �ִ´�.
		);

		// ���Ͽ��� �л����� �б�
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
					nmttdispinfo->hdr.hwndFrom, // ������ �ڵ�
					-1, // �˻��� ������ �ε���
					LVNI_SELECTED // �˻� ����
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

		case IDC_BUTTON2: // �л� ���� ��ư
		{
			g_studentList.RemoveStudent(g_nSelectedStudentIndex);

			g_nSelectedStudentIndex = -1;
			UpdateStudentsListView(hWnd);
			UpdateDeleteButtonState(hWnd, g_nSelectedStudentIndex != -1);
		}
		break;

		case IDC_BUTTON4: // �л� �������� ��ư
		{
			g_studentList.SaveStudents();
		}
		break;

		case IDC_BUTTON3: // �л� �߰� ��ư
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
				strcpy_s(g_strLoginId, id);
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

void SetLoginInfo(HWND hWnd) {
	SetDlgItemText(hWnd, IDC_EDIT4, g_strLoginId);
}

void UpdateStudentsListView(HWND hWnd)
{
	HWND hListView = GetDlgItem(hWnd, IDC_LIST1);

	ListView_DeleteAllItems(hListView);

	// ������ �߰�
	LVITEM item = {};
	item.mask = LVIF_TEXT;
	item.iSubItem = 0; // �������� ó�� �߰��ϹǷ� 0��° ����������� �����Ѵ�.
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
		ListView_InsertItem(hListView, &item); // ������ �߰�0
		ListView_SetItemText(hListView, itemCount/*item idx*/, 1/*subitem idx*/, it->Name); // ��������� �߰�0

		++it;
	}
}

void UpdateDeleteButtonState(HWND hWnd, bool bEnable) {
	EnableWindow(GetDlgItem(hWnd, IDC_BUTTON2), bEnable);
}