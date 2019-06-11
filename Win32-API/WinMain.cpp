#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
//#include <iostream>
#include <string>
#include "resource.h"

HINSTANCE g_hInstance;
HWND g_hMainWnd;

TCHAR g_str[100];
UINT g_strIndex = 0;
POINT g_movableTextPosition = { 10, 150 };
BOOL g_bF1KeyDown = FALSE;
BOOL g_bDragging = false;
POINT g_dragStartPosition = { 0, 0 };
UINT g_nTimerCount1 = 0;
UINT g_nTimerCount2 = 0;

UINT g_nDialogX = 0;
UINT g_nDialogY = 0;
TCHAR g_szDialogStr[100] = { 0, };

HWND g_hModelessDialog;

TCHAR BoolToChar(BOOL bBool);
VOID CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR id, DWORD time);
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc_Modeless(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hInstance = hInstance;


	// 0. ���ҽ� ���Ͽ��� ��Ʈ�� ��������
	TCHAR szWindowName[100];
	LoadString(hInstance, IDS_TITLE, szWindowName, 100);

	// 1. Window Class ����ü ����

	// Window Class ����ü ����
	WNDCLASSEX wcex;

	// WNDCLASSEX�� ũ�⸦ �Է�
	// cb : count of bytes
	wcex.cbSize = sizeof(WNDCLASSEX);

	// ������ ��� ���� ����. ����, ���� ����� ���ϸ� ��ü�� �ٽ� �׸���.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// �޽��� ó���� ���� �Լ�
	wcex.lpfnWndProc = WndProc;

	// ������ 'Window Class'�� ����ϴ� Window���� ������ �� �ִ� �޸� ũ�⸦ ����
	// 40����Ʈ���� ����.
	// SetClassWord, SetClassLong, GetClassWord, GetClassLong �Լ��� ����Ͽ� ���
	// ���ȿ� ����ϰ� ũ�⵵ �������̶� ���� ������� ����.
	wcex.cbClsExtra = 0;

	// �� 'Window Class'�� ����ϴ� Window�� ���� ���� �޸𸮸� �߰�. ������ 'Window Class'�� ����ص� �������� �ʴ´�.
	// 40����Ʈ���� ����
	// SetWindowLong, GetWindowLong �Լ��� ����Ͽ� ���
	// ���� ������� ����.
	wcex.cbWndExtra = 0;

	// ������ �ν��Ͻ�
	wcex.hInstance = hInstance;

	// �⺻ ������ ����, OS���� �̸� ������ �������� ����� �� �ִ�.
	// IDI : ID for an icon
	// ��� 1. ���ҽ��� �̸��� ���ڿ� �����ͷ� ����
	wcex.hIcon = LoadIcon(NULL, "resourceName");
	// ��� 2. Resource.h�� define�� ���ҽ� ���̵� MAKEINTRESOURCE�� �̿��� ���ڿ� ������ �������� ����
	// ���ҽ� ���̵�� ����� ���ڿ� �����ʹ� ��ȿ���� ������ OS�� �̸� ID������ ����Ͽ� ���ڿ��� ã�´�.(?)
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // IDI_APPLICATION : �ü���� �̸� ���ǵ� �⺻ ������
	// ��� 3. �̸����ǵ� �������� �ƴ� ��� MAKEINTRESOURCE ��ũ�θ� ����� ���� ���̵��� ���ڷ� �ٲ��ش�.
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // IDI_APPLICATION : �ü���� �̸� ���ǵ� �⺻ ������

	// �⺻ ���� ������ ����
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wcex.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

	// �⺻ Ŀ�� ����, OS���� �̸� ������ Ŀ���� ����� �� �ִ�.
	// IDC : ID for an cursor
	// ��� 1. �ü���� ���� ����Ѵ�. instance�� NULL�� �ش�.
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	//wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	// ��� 2. ������ Ŀ���� ����Ѵ�.
	//wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));


	// �������� ��� ���� ����
	// ��� 1. CreateSolidBrush()�� �귯�ø� ���� �����Ͽ� �Ҵ�. 
	// ���α׷� ���� �� �ݵ�� �����ؾ��Ѵ�.
	HBRUSH brushBg = CreateSolidBrush(RGB(0, 0, 0));
	wcex.hbrBackground = brushBg;
	// ���  2. OS�� �̸� ������ ����� ���
	// COLOR_WINDOW : ȸ��
	// COLOR_WINDOW + 1 : ���
	// COLOR_WINDOW + 2 : ��ο� ȸ��
	// COLOR_WINDOW + 3 : ������
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// ��� 3. OS�� �̸� ����� ���� Stock Resource�� ������ ���
	// OS�� ���󵵰� ���� �ڿ��� �̸� ����� ���µ� �� �ڿ��� Stock Resource �Ǵ� Stock Object�� �Ѵ�.
	// �� �ڿ��� WHITE_BRUSH�� �ش��ϴ� �ڿ��� �ڵ鰪�� ��� �������� �����Ѵ�.
	// ��ȯ���� HGDIOBJ�̹Ƿ� ����ȯ�Ͽ� ����Ѵ�.
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// Window Class�� �̸��� �����ϰ� �� �̸����� Window Class�� �����ϱ� ������ �ٸ� Window Class�� �ߺ����� �ʰ� �� �����Ѵ�.
	// ��ҹ��ڸ� �������� �ʴ´�. 
	LPCTSTR strClassName = TEXT("Win32");
	wcex.lpszClassName = strClassName;

	// �޴� �̸�
	//wcex.lpszMenuName = MAKEINTRESOURCE(106);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);


	// 2. Window Class ����ü ���
	RegisterClassEx(&wcex);


	// 3. Window ����
	HWND hWnd = CreateWindow(
		strClassName, // LPCSTR lpClassName : Window Class �̸�
		szWindowName, // LPCSTR lpWindowName : ������ Ÿ��Ʋ �̸�
		//TEXT("Hello World"), // LPCSTR lpWindowName : ������ Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, // DWORD dwStyle : ������ ��Ÿ��, dw(DWORD)
		CW_USEDEFAULT, 0, // int X, int Y : ������ x, y ��ǥ, CW(Create Window), CW_USEDEFAULT : �ü���� ���� �⺻���� ����Ѵ�.
		CW_USEDEFAULT, 11, // int nWidth, int nHeight : ������ ����, ���� ũ��(Ŭ���̾�Ʈ ���� ����)
		nullptr, // HWND hWndParent : �θ� ������ �ڵ�
		nullptr, // HMENU hMenu : �޴� �ڵ�
		hInstance, // HINSTANCE hInstance : ���� ���α׷� �ν��Ͻ�
		nullptr // LPVOID lpParam : ���� ������ ����, ���������� ������� �ʴ´�.
	);
	g_hMainWnd = hWnd;


	// 4. ������ ���
	// ��� 1. ���α׷� ���� �� �Ѿ�� ShowWindow Commands�� ����Ѵ�.
	ShowWindow(hWnd, nCmdShow);
	// ��� 2. �̸� ���ǵ� ShowWindow Commands�� ����Ѵ�.
	ShowWindow(hWnd, SW_SHOW);
	// WM_PAINT �޽����� �߻����� �����츦 �׸���.
	UpdateWindow(hWnd);


	// 5. �޽��� ����
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	LPPAINTSTRUCT lpps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE: // �����찡 ó�� �������� �� �߻�
		SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 2000, TimerProc);

		g_nDialogX = 10;
		g_nDialogY = 300;
		_stprintf_s(g_szDialogStr, TEXT("Dialog"));
		break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			g_nTimerCount1++;
			break;

			/*case 2:
				g_nTimerCount2++;
				break;*/

		default:
			break;
		}
		InvalidateRect(hWnd, NULL, false);
	}
	break;
	case WM_COMMAND:
	{
		WORD menuId = LOWORD(wParam);
		WORD menuEvent = HIWORD(wParam);
		const UINT strLength = 100;
		TCHAR strBuffer[strLength];
		_stprintf_s(strBuffer, strLength, "id:%d, event:%d", menuId, menuEvent);

		switch (menuId)
		{
		case ID_40001:
		case ID_40003:
		case ID_40004:
			//_stprintf_s(strBuffer, strLength, "%d", menuId);
			break;
		default:
			break;
		}

		MessageBox(hWnd, strBuffer, "Title~~", MB_OK);
	}
	break;
	case WM_SETCURSOR:
		HCURSOR hCursor;
		hCursor = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
		SetCursor(hCursor);
		break;
	case WM_CHAR:
	{
		g_str[g_strIndex++] = wParam;
		InvalidateRect(NULL, NULL, TRUE);
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
			g_movableTextPosition.x -= 1;
			break;
		case VK_RIGHT:
			g_movableTextPosition.x += 1;
			break;
		case VK_UP:
			g_movableTextPosition.y -= 1;
			break;
		case VK_DOWN:
			g_movableTextPosition.y += 1;
			break;
		default:
			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		COLORREF color = COLORREF RGB(255, 0, 0);
		BYTE r = GetRValue(color);

		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkColor(hdc, RGB(0, 255, 255));

		TextOut(hdc, 10, 10, TEXT("hihi"), _tcslen(TEXT("hihi")));
		TextOut(hdc, 10, 100, g_str, _tcslen(g_str));
		TextOut(hdc, g_movableTextPosition.x, g_movableTextPosition.y, TEXT("KeyDown"), _tcslen(TEXT("KeyDown")));
		TextOut(hdc, 10, 200, g_bF1KeyDown ? "t" : "f", _tcslen(TEXT("a")));
		TextOut(hdc, 10, 250, std::to_string(g_nTimerCount1).c_str(), 10);
		TextOut(hdc, 10, 260, std::to_string(g_nTimerCount2).c_str(), 10);
		TextOut(hdc, g_nDialogX, g_nDialogY, g_szDialogStr, _tcslen(g_szDialogStr));

		SetPixel(hdc, 150, 150, RGB(255, 0, 0));
		SetPixel(hdc, 151, 150, RGB(0, 255, 0));
		SetPixel(hdc, 152, 150, RGB(0, 0, 255));

		COLORREF color1 = GetPixel(hdc, 150, 150);
		COLORREF color2 = GetPixel(hdc, 149, 149);

		// ========== Brush ========== 
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		// ========== Brush ========== end

		// ========== Pen ==========
		// Pen ����
		HPEN hPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
		// DC�� �� ����
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, 100, 100, NULL);
		LineTo(hdc, 200, 100);

		Rectangle(hdc, 200, 100, 250, 150);

		Ellipse(hdc, 250, 150, 300, 200);

		SelectObject(hdc, hOldPen);
		BOOL bDeletePenResult = DeleteObject(hPen);
		//LineTo(hdc, 100, 200);

		TCHAR szDeletePenResult[] = TEXT("DeletePenResult: %s");
		//TCHAR szDeletePenResult[10] = { 0, };
		_stprintf_s(szDeletePenResult, bDeletePenResult ? TEXT("t") : TEXT("f"));
		TextOut(hdc, 100, 205, szDeletePenResult, _tcslen(szDeletePenResult));

		// ========== Pen ========== end


		// ========== Bitmap ==========
		// ��Ʈ�� ���
		BITMAP resBitmapHeader; // ���ҽ� ��Ʈ�� ���
		BITMAP fileBitmapHeader; // ���� ��Ʈ�� ���

		// ���ҽ��� ��Ʈ�� �ҷ�����
		HBITMAP hResBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		// ������ ��Ʈ�� �ҷ�����, ������ ������Ʈ ������ �����̴�.(������Ʈ ���� ���� Debug ������ ���� �ʴ´�)
		HBITMAP hFileBitmap = (HBITMAP)LoadImage(NULL, TEXT("zzal.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// ��Ʈ�� ����� �о����
		GetObject(hResBitmap, sizeof(BITMAP), &resBitmapHeader);
		GetObject(hFileBitmap, sizeof(BITMAP), &fileBitmapHeader);

		// Memory DC ����
		HDC hResMemDC = CreateCompatibleDC(hdc);
		HDC hFileMemDC = CreateCompatibleDC(hdc);

		// Memory DC�� ��Ʈ�� ����
		SelectObject(hResMemDC, hResBitmap);
		SelectObject(hFileMemDC, hFileBitmap);

		// Memory DC�� ȭ�� DC�� ����
		BitBlt(hdc, 300, 300, resBitmapHeader.bmWidth, resBitmapHeader.bmHeight, hResMemDC, 0, 0, SRCCOPY);

		//BitBlt(hdc, 300 + resBitmapHeader.bmWidth, 300, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, hFileMemDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, 300 + resBitmapHeader.bmWidth, 300, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, hFileMemDC, 0, 0, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, RGB(231, 223, 220));


		// �����ߴ� ��Ʈ�ʰ� Memory DC�� ����
		DeleteObject(hResBitmap);
		DeleteObject(hFileBitmap);
		DeleteDC(hResMemDC);
		DeleteDC(hFileMemDC);
		// ========== Bitmap ========== end


		EndPaint(hWnd, &ps);

		hdc = GetDC(hWnd);
		ReleaseDC(hWnd, hdc);
	}

	break;
	case WM_LBUTTONDOWN:
	{
		hdc = GetDC(hWnd);
		TextOut(hdc, 100, 100, TEXT("Clicked"), _tcslen(TEXT("Clicked")));

		RECT rect = { 0, 0, 300, 300 };
		SetPixel(hdc, rand() % 300, rand() % 300, RGB(255, 0, 0));
		InvalidateRect(hWnd, &rect, TRUE);
		//ReleaseDC(hWnd, hdc);

		if (GetAsyncKeyState(VK_F1) & 0x8000) {
			g_bF1KeyDown = TRUE;
		}
		else {
			g_bF1KeyDown = FALSE;
		}

		WORD x = LOWORD(lParam);
		WORD y = HIWORD(lParam);
		g_dragStartPosition = { x, y };
		g_bDragging = TRUE;
	}
	break;
	case WM_LBUTTONUP:
	{
		g_bDragging = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		// �巡���Ͽ� �簢�� �׸��� ��� 1.
		if (g_bDragging) {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			HDC hdc = GetDC(hWnd);
			Rectangle(hdc, g_dragStartPosition.x, g_dragStartPosition.y, x, y);
			ReleaseDC(hWnd, hdc);
		}

		// �巡���Ͽ� �簢�� �׸��� ��� 2.
		if (wParam == MK_LBUTTON) {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			HDC hdc = GetDC(hWnd);
			Rectangle(hdc, g_dragStartPosition.x, g_dragStartPosition.y, x, y);
			ReleaseDC(hWnd, hdc);
		}
	}
	break;
	case WM_RBUTTONDOWN:
		//hdc = GetDC(hWnd);
		//TextOut(hdc, 100, 100, TEXT("Release"), _tcslen(TEXT("Release")));
		//ReleaseDC(hWnd, hdc);

		// �𵨸��� ���̾�α� ����
		// IsWindow() �Լ��� ���� ��ȿ�� ������ �ڵ����� Ȯ���Ѵ�.
		if (IsWindow(g_hModelessDialog) == FALSE) {
			// CreateDialog()�� ������ ���̾�α��� �ڵ��� �ٷ� ��ȯ�Ѵ�.
			g_hModelessDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DialogProc_Modeless);

			// ���̾�α׸� ���������� ���� ������ �ʱ� ������ ȭ�鿡 ���̵��� �Ѵ�.
			// ���α׿� WS_VISIBLE ��Ÿ���� �־��ٸ� 
			// ShowWindow() �Լ��� ȣ������ �ʾƵ� ������
			// ����Ʈ�� WS_VISIBLE ��Ÿ���� �ƴϹǷ� ȣ���Ѵ�.
			ShowWindow(g_hModelessDialog, SW_SHOW);
		}
		break;
	case WM_MBUTTONDOWN:
	{
		// ��� ���̾�α� �ڽ� ����
		// DialogBox�� ��ȯ���� ���� ����/��Ҹ� �Ǵ��Ͽ� ó���Ѵ�.
		// ��ȯ���� EndDialog�� �ι�° �Ķ���ͷ� �ѱ�� ���� ��ȯ�ȴ�.
		if (DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc) != 0) {
			InvalidateRect(hWnd, NULL, TRUE);
		};
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0); // GetMessage() �Լ��� 0�� ��ȯ�ϰ� �ȴ�.
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

TCHAR BoolToChar(BOOL bBool) {
	return bBool ? 't' : 'f';
}

VOID CALLBACK TimerProc(HWND hWnd, UINT message, UINT_PTR id, DWORD time) {
	g_nTimerCount2++;
}

// ��� ���̾�α� ���ν���
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		// ���̾�α� �ʱ�ȭ �� �� ��Ʈ�ѷ��� �ʱⰪ�� �־��ش�.
		SetDlgItemInt(hWnd, IDC_X, g_nDialogX, FALSE);
		SetDlgItemInt(hWnd, IDC_Y, g_nDialogY, FALSE);
		SetDlgItemText(hWnd, IDC_STR, g_szDialogStr);
		return true;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			// �۾��� ������ ���̾�α�â�� ���������� ���� �� ��Ʈ�ѷ��� �ִ� ���� �־��ش�.
			g_nDialogX = GetDlgItemInt(hWnd, IDC_X, NULL, FALSE);
			g_nDialogY = GetDlgItemInt(hWnd, IDC_Y, NULL, FALSE);
			GetDlgItemText(hWnd, IDC_STR, g_szDialogStr, 100);
			EndDialog(hWnd, 1); // DialogBox()�� ���ϰ��� 1�� �ȴ�.
			break;
		case IDCANCEL:
			// ���̾�α� �۾��� ����� ��� �״�� ������.
			EndDialog(hWnd, FALSE); // DialogBox()�� ���ϰ��� 0�� �ȴ�.
			return TRUE;
		default:
			break;
		}
	default:
		return FALSE;
		break;
	}
	return FALSE;
}

// �𵨸��� ���̾�α� ���ν���
INT_PTR CALLBACK DialogProc_Modeless(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hWnd, IDC_X, g_nDialogX, FALSE);
		SetDlgItemInt(hWnd, IDC_Y, g_nDialogY, FALSE);
		SetDlgItemText(hWnd, IDC_STR, g_szDialogStr);
		return true;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_CHANGE:
			g_nDialogX = GetDlgItemInt(hWnd, IDC_X, NULL, FALSE);
			g_nDialogY = GetDlgItemInt(hWnd, IDC_Y, NULL, FALSE);
			GetDlgItemText(hWnd, IDC_STR, g_szDialogStr, 100);
			InvalidateRect(g_hMainWnd, NULL, TRUE); // ������ ������ ������������ ȭ�鿡 �ݿ�
			break;
		case ID_CLOSE:
			DestroyWindow(g_hModelessDialog); // CreateDialog()�� �������Ƿ� DestroyWindow()�� �ݴ´�.
			//EndDialog(hWnd, FALSE); // ������������ �ٽ� ������ �ʴ´�.
			return TRUE;
		default:
			break;
		}
	default:
		return FALSE;
		break;
	}
	return FALSE;
}