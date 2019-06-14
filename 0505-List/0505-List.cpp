#include "Windows.h"
#include "resource.h"
//#include "Windowsx.h" // include ���ص� ListView ��밡���ϳ�...
#include "Commctrl.h" // LVCOLUMN
#include <stdio.h>

const UINT nMax_Char = 100;

HWND g_hList = nullptr;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_INITDIALOG:
	{
		g_hList = GetDlgItem(hWnd, IDC_LIST1);

		// �÷� : �׸�
		// ������ : ��(Row)
		// ��������� : ��(Column)

		// ===== List�� �÷� �߰� =====
		// List �Ӽ��� View�� Report�� �����ؾ� �÷��� �߰��� �� �ִ�.
		char colText0[] = "col0";
		char colText1[] = "col1";

		LVCOLUMN col = {};
		col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		col.fmt = LVCFMT_LEFT;
		col.cx = 100;
		col.pszText = colText0;
		ListView_InsertColumn(g_hList, 0, &col); // �÷� �߰�0

		col.pszText = colText1;
		ListView_InsertColumn(g_hList, 1, &col); // �÷� �߰�1
		// ===== List�� �÷� �߰� ===== end

		// ===== List�� ������ �߰� =====
		char itemText0[] = "item0";
		char itemText1[] = "item1";
		LVITEM item = {};
		item.mask = LVIF_TEXT;
		item.iItem = 0;
		item.iSubItem = 0; // �������� ó�� �߰��ϹǷ� 0��° ����������� �����Ѵ�.
		item.state;
		item.stateMask;
		item.pszText = itemText0;
		ListView_InsertItem(g_hList, &item); // ������ �߰�0

		int itemCount = ListView_GetItemCount(g_hList);
		item.iItem = itemCount;
		item.pszText = itemText1;
		ListView_InsertItem(g_hList, &item); // ������ �߰�1
		// ===== List�� ������ �߰� ===== end

		// ===== List�� ��������� �߰� =====
		char suhbitemText0[] = "subitem0";
		char suhbitemText1[] = "subitem1";
		ListView_SetItemText(g_hList, 0/*item idx*/, 1/*subitem idx*/, suhbitemText0); // ��������� �߰�0
		ListView_SetItemText(g_hList, 1, 1, suhbitemText1); // ��������� �߰�1(�÷��� ���ٸ� ȭ�鿡 ������ �ʴ´�)
		// ===== List�� ��������� �߰� ===== end

		// ===== List �� ���� =====
		// �⺻���� ù ��° ����������� �ؽ�Ʈ ������ ���õ�
		ListView_SetExtendedListViewStyle(
			g_hList,
			LVS_EX_FULLROWSELECT // ������ ��ü�� Ŭ���ǵ��� �Ѵ�.
			| LVS_EX_GRIDLINES // ��������� ���̿� �׸��� ������ �ִ´�.
		);
		// ===== List �� ���� ===== end

		return true;
	}
	break;

	// ǥ�� ��Ʈ���� WM_COMMAND�� ���� �޽����� ����
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1: // Delete button
		{
			UINT itemIndex = ListView_GetNextItem(
				g_hList, // ������ �ڵ�
				-1, // �˻��� ������ �ε���
				LVNI_SELECTED // �˻� ����
			);
			if (itemIndex != -1) {
				ListView_DeleteItem(g_hList, itemIndex); // ������ �ε����� ������ ����
			}
		}
		break;

		case IDC_BUTTON2: // Delete All button
		{
			if (MessageBox(hWnd, "Remove all?", "", MB_OKCANCEL) == IDOK) {
				ListView_DeleteAllItems(g_hList); // ListView�� ��� ������ ����
			};
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

	// ���� ��Ʈ���� ���� �޽����� WM_NOTIFY�� �޴´�.
	case WM_NOTIFY:
	{
		// ListView�� ���� �޽��� �ޱ�
		if (wParam == IDC_LIST1) {
			NMHDR *pnmhdr = nullptr;

			// NMHDR�� ��� ���1.
			NMTTDISPINFO *nmttdispinfo = (NMTTDISPINFO*)lParam;
			pnmhdr = &(nmttdispinfo->hdr);

			// NMHDR�� ��� ���2.
			// NMTTDISPINFO�� NMHDR�� Ȯ���ϴ� ����ü�μ�
			// ù ��������� NMHDR�� ������ �ֱ� ������ NMHDR �����ε� ĳ������ �����ϴ�.
			pnmhdr = (NMHDR*)lParam;

			pnmhdr->hwndFrom; // ������ �ڵ�
			pnmhdr->idFrom; // ��Ʈ�� ���̵�
			pnmhdr->code; // ���� �ڵ�

			if (pnmhdr->code == NM_CLICK) {
				// Ŭ���� ������ �ε��� �˾Ƴ���
				UINT itemIndex = ListView_GetNextItem(
					pnmhdr->hwndFrom, // ������ �ڵ�
					-1, // �˻��� ������ �ε���
					LVNI_SELECTED // �˻� ����
				);

				if (itemIndex == -1) {
					// ���� ����
				}
				else {
					// ����������� �ؽ�Ʈ ��������
					char result[nMax_Char] = {};
					char subItem0[nMax_Char] = {};
					char subItem1[nMax_Char] = {};
					ListView_GetItemText(pnmhdr->hwndFrom, itemIndex, 0, subItem0, nMax_Char);
					ListView_GetItemText(pnmhdr->hwndFrom, itemIndex, 1, subItem1, nMax_Char);
					sprintf_s(result, "%s, %s", subItem0, subItem1);

					// ���
					SetDlgItemText(hWnd, IDC_EDIT1, result);
				}
			}
			else if (pnmhdr->code == LVN_ITEMCHANGED) {
				UINT itemIndex = ListView_GetNextItem(
					pnmhdr->hwndFrom, // ������ �ڵ�
					-1, // �˻��� ������ �ε���
					LVNI_SELECTED // �˻� ����
				);
				if (itemIndex == -1) {
					SetDlgItemText(hWnd, IDC_EDIT1, "");
				}
			}
		}
	}
	break;

	default:
		break;
	}

	return false;
}