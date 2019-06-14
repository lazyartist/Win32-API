#include "Windows.h"
#include "resource.h"
//#include "Windowsx.h" // include 안해도 ListView 사용가능하네...
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

		// 컬럼 : 항목
		// 아이템 : 행(Row)
		// 서브아이템 : 열(Column)

		// ===== List에 컬럼 추가 =====
		// List 속성의 View를 Report로 설정해야 컬럼을 추가할 수 있다.
		char colText0[] = "col0";
		char colText1[] = "col1";

		LVCOLUMN col = {};
		col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		col.fmt = LVCFMT_LEFT;
		col.cx = 100;
		col.pszText = colText0;
		ListView_InsertColumn(g_hList, 0, &col); // 컬럼 추가0

		col.pszText = colText1;
		ListView_InsertColumn(g_hList, 1, &col); // 컬럼 추가1
		// ===== List에 컬럼 추가 ===== end

		// ===== List에 아이템 추가 =====
		char itemText0[] = "item0";
		char itemText1[] = "item1";
		LVITEM item = {};
		item.mask = LVIF_TEXT;
		item.iItem = 0;
		item.iSubItem = 0; // 아이템을 처음 추가하므로 0번째 서브아이템을 선택한다.
		item.state;
		item.stateMask;
		item.pszText = itemText0;
		ListView_InsertItem(g_hList, &item); // 아이템 추가0

		int itemCount = ListView_GetItemCount(g_hList);
		item.iItem = itemCount;
		item.pszText = itemText1;
		ListView_InsertItem(g_hList, &item); // 아이템 추가1
		// ===== List에 아이템 추가 ===== end

		// ===== List에 서브아이템 추가 =====
		char suhbitemText0[] = "subitem0";
		char suhbitemText1[] = "subitem1";
		ListView_SetItemText(g_hList, 0/*item idx*/, 1/*subitem idx*/, suhbitemText0); // 서브아이템 추가0
		ListView_SetItemText(g_hList, 1, 1, suhbitemText1); // 서브아이템 추가1(컬럼이 없다면 화면에 보이지 않는다)
		// ===== List에 서브아이템 추가 ===== end

		// ===== List 뷰 설정 =====
		// 기본값은 첫 번째 서브아이템의 텍스트 영역만 선택됨
		ListView_SetExtendedListViewStyle(
			g_hList,
			LVS_EX_FULLROWSELECT // 아이템 전체가 클릭되도록 한다.
			| LVS_EX_GRIDLINES // 서브아이템 사이에 그리드 라인을 넣는다.
		);
		// ===== List 뷰 설정 ===== end

		return true;
	}
	break;

	// 표준 컨트롤은 WM_COMMAND를 통해 메시지를 통지
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1: // Delete button
		{
			UINT itemIndex = ListView_GetNextItem(
				g_hList, // 윈도우 핸들
				-1, // 검색을 시작할 인덱스
				LVNI_SELECTED // 검색 조건
			);
			if (itemIndex != -1) {
				ListView_DeleteItem(g_hList, itemIndex); // 지정한 인덱스의 아이템 제거
			}
		}
		break;

		case IDC_BUTTON2: // Delete All button
		{
			if (MessageBox(hWnd, "Remove all?", "", MB_OKCANCEL) == IDOK) {
				ListView_DeleteAllItems(g_hList); // ListView의 모든 아이템 제거
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

	// 공통 컨트롤의 통지 메시지는 WM_NOTIFY로 받는다.
	case WM_NOTIFY:
	{
		// ListView의 통지 메시지 받기
		if (wParam == IDC_LIST1) {
			NMHDR *pnmhdr = nullptr;

			// NMHDR을 얻는 방법1.
			NMTTDISPINFO *nmttdispinfo = (NMTTDISPINFO*)lParam;
			pnmhdr = &(nmttdispinfo->hdr);

			// NMHDR을 얻는 방법2.
			// NMTTDISPINFO는 NMHDR을 확장하는 구조체로서
			// 첫 멤버변수로 NMHDR을 가지고 있기 때문에 NMHDR 형으로도 캐스팅이 가능하다.
			pnmhdr = (NMHDR*)lParam;

			pnmhdr->hwndFrom; // 윈도우 핸들
			pnmhdr->idFrom; // 컨트롤 아이디
			pnmhdr->code; // 통지 코드

			if (pnmhdr->code == NM_CLICK) {
				// 클릭된 아이템 인덱스 알아내기
				UINT itemIndex = ListView_GetNextItem(
					pnmhdr->hwndFrom, // 윈도우 핸들
					-1, // 검색을 시작할 인덱스
					LVNI_SELECTED // 검색 조건
				);

				if (itemIndex == -1) {
					// 선택 없음
				}
				else {
					// 서브아이템의 텍스트 가져오기
					char result[nMax_Char] = {};
					char subItem0[nMax_Char] = {};
					char subItem1[nMax_Char] = {};
					ListView_GetItemText(pnmhdr->hwndFrom, itemIndex, 0, subItem0, nMax_Char);
					ListView_GetItemText(pnmhdr->hwndFrom, itemIndex, 1, subItem1, nMax_Char);
					sprintf_s(result, "%s, %s", subItem0, subItem1);

					// 출력
					SetDlgItemText(hWnd, IDC_EDIT1, result);
				}
			}
			else if (pnmhdr->code == LVN_ITEMCHANGED) {
				UINT itemIndex = ListView_GetNextItem(
					pnmhdr->hwndFrom, // 윈도우 핸들
					-1, // 검색을 시작할 인덱스
					LVNI_SELECTED // 검색 조건
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