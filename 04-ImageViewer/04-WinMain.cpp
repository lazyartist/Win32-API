#include "Windows.h"
#include "resource.h"

char g_szFile[MAX_PATH];
char g_szFileTitle[MAX_PATH];
HDC g_hMemDC;
BITMAP g_bitmapHeader;

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

	return 0;
}

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, g_bitmapHeader.bmWidth, g_bitmapHeader.bmHeight, g_hMemDC, 0, 0, SRCCOPY);
		TextOut(hdc, 10, 200, g_szFile, strlen(g_szFile));
		TextOut(hdc, 10, 230, g_szFileTitle, strlen(g_szFileTitle));

		EndPaint(hWnd, &ps);
	}

	break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		case IDC_OPEN:
		{
			OPENFILENAME ofn = { 0, };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = "Image File(*.bmp)\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = g_szFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFileTitle = g_szFileTitle;
			ofn.nMaxFileTitle = MAX_PATH;

			if (GetOpenFileName(&ofn) != 0) {
				strcpy_s(g_szFile, ofn.lpstrFile);
				strcpy_s(g_szFileTitle, ofn.lpstrFileTitle);

				// ===== Image Load =====
				// LoadImage
				HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, g_szFileTitle, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

				// bitmap 정보 읽기
				GetObject(hBitmap, sizeof(BITMAP), &g_bitmapHeader);

				// get current dc
				HDC hdc = GetDC(hWnd);

				// memory dc
				if (g_hMemDC != NULL) { // 해제하지 않아도 에러는 안난다.
					DeleteDC(g_hMemDC);
				}
				g_hMemDC = CreateCompatibleDC(hdc);

				// select object
				SelectObject(g_hMemDC, hBitmap);

				DeleteObject(hBitmap);
				ReleaseDC(hWnd, hdc);

				InvalidateRect(hWnd, NULL, true);
				// ===== Image Load ===== end
			};
		}
		default:
			break;
		}
		break;
	case WM_DESTROY:
		if (g_hMemDC != NULL) { // 해제하지 않아도 에러는 안난다.
			DeleteDC(g_hMemDC);
		}
		break;
	default:
		break;
	}

	return 0;
}