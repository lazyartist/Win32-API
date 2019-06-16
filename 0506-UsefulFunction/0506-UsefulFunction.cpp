#include "stdafx.h"
#include "Windowsx.h" // GET_X_LPARAM, GET_Y_LPARAM
#include "0506-UsefulFunction.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
RECT g_rectMouse;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK	ImageDialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY0506USEFULFUNCTION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0506USEFULFUNCTION));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0506USEFULFUNCTION));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY0506USEFULFUNCTION);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	// ===== AdjustWindowRect() =====
	RECT clientRect = { 0, 0, 199, 199 }; // 클라이언트 영역 크기 지정
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, true);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, nullptr, nullptr, hInstance, nullptr);
	// ===== AdjustWindowRect() ===== end

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// ===== SetROP2() =====
		// SetROP2용 모델리스 다이얼로그 생성
		HWND hImgDlgWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, ImageDialogProc);
		ShowWindow(hImgDlgWnd, SW_SHOW);

		// 생성된 다이얼로그를 메인 윈도우 옆에 띄움
		RECT rectMainWindow = {}; // 메인 윈도우 크기, 위치
		RECT rectDlgWindow = {}; // 생성한 다이얼로그 크기, 위치
		GetWindowRect(hWnd, &rectMainWindow);
		GetWindowRect(hImgDlgWnd, &rectDlgWindow);
		MoveWindow(hImgDlgWnd, rectMainWindow.right + 10, rectMainWindow.top,
			rectDlgWindow.right - rectDlgWindow.left, rectDlgWindow.bottom - rectDlgWindow.top, false);
		// ===== SetROP2() ===== end
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
		Rectangle(hdc, 0, 0, 199, 199);

		// ===== GetClientRect() =====
		RECT clientRect = {}; // 클라이언트 크기를 저장할 RECT
		GetClientRect(hWnd, &clientRect);
		// ===== GetClientRect() ===== end


		// ===== GetWindowRect() =====
		RECT windowRect = {}; // 윈도우 크기를 저장할 RECT
		GetWindowRect(hWnd, &windowRect);
		// ===== GetWindowRect() ===== end


		// ===== CreateCompatibleBitmap() =====
		// 메모리 DC 생성
		HDC hMemDC = CreateCompatibleDC(hdc);

		// 메모리 DC에 자동으로 연결된 비트맵 정보 확인
		HGDIOBJ hOldBitmap = GetCurrentObject(hMemDC, OBJ_BITMAP);
		BITMAP hOldBitmapInfo = {};
		GetObject(hOldBitmap, sizeof(BITMAP), &hOldBitmapInfo); // 색상 1비트, 1픽셀 비트맵이다.

		// 비트맵 생성
		// hdc의 비트맵의 색상과 같은 비트맵을 만든다.
		// 따라서 현재 제대로된 비트맵이 없는 hMemDC를 hdc의 인자로 넘기면 1비트 색상을 사용하는 비트맵이 만들어진다.
		HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 100, 100);

		// 메모리 DC에 비트맵 적용
		// DC에 비트맵이 있어야 GDI 함수를 이용하여 그림을 그릴 수 있다.
		SelectObject(hMemDC, hBitmap);

		// 메모리 DC를 단일 색생으로 채움
		PatBlt(hMemDC, 0, 0, 100, 100, WHITENESS);

		// 메모리 DC에 사각형 그림
		Rectangle(hMemDC, 0, 0, 50, 50);

		// 화면DC에 메모리DC 적용
		BitBlt(hdc, 50, 50, 100, 100,
			hMemDC, 0, 0, SRCCOPY);
		// ===== CreateCompatibleBitmap() ===== end


		SelectObject(hdc, hOldPen);

		EndPaint(hWnd, &ps);

		DeleteObject(hPen);
	}
	break;

	case WM_KEYDOWN:
	{
		if (wParam == VK_RIGHT) {
			// ===== MoveWindow() =====
			RECT windowRect = {}; // 윈도우 크기를 저장할 RECT
			GetWindowRect(hWnd, &windowRect);
			MoveWindow(hWnd, windowRect.left + 1, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, false);
			// ===== MoveWindow() ===== end
		}
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK ImageDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_PAINT:
	{
		PAINTSTRUCT ps = {};
		HDC hdc = BeginPaint(hDlg, &ps);

		// load bitmap
		HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, "zzal.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		BITMAP bitmapHeader = {};
		GetObject(hBitmap, sizeof(BITMAP), &bitmapHeader);

		// draw bitmap
		HDC hMemDC = CreateCompatibleDC(hdc);
		SelectObject(hMemDC, hBitmap);

		// 비트맵을 화면 dc에 옮김
		BitBlt(hdc, 0, 0, bitmapHeader.bmWidth, bitmapHeader.bmHeight,
			hMemDC, 0, 0, SRCCOPY);


		// ===== SetROP2() =====
		// 현재 비트맵에 새로 그려지는 그림의 겹쳐지는 방식을 설정
		SetROP2(hdc, R2_NOTXORPEN); // 외곽은 검은색, 내부는 비어있는 사각형
		// ===== SetROP2() ===== end


		//HBRUSH hOlfBrush = nullptr;
			Rectangle(hdc, g_rectMouse.left, g_rectMouse.top, g_rectMouse.right, g_rectMouse.bottom);

		// ===== PtInRect() =====
		// 200, 150 위치에 점하니 찍고 이 영역을 마우스 드래그 영역이 포함하면 빨간 사각형 그리기
		SetPixel(hdc, 200, 150, RGB(255, 0, 0));
		if (PtInRect(&g_rectMouse/*영역 RECT*/, { 200, 150 }/*위치 POINT*/) != 0) {
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, g_rectMouse.left, g_rectMouse.top, g_rectMouse.right, g_rectMouse.bottom);
			DeleteObject(SelectObject(hdc, hOldBrush));
		}
		// ===== PtInRect() ===== end


		// ===== IntersectRect() ===== 
		// 비트맵 영역과 마우스 드래그 영역이 겹치면 녹색 사각형 그리기
		RECT intersectRect = {};
		RECT bitmapRect = { 0, 0, bitmapHeader.bmWidth, bitmapHeader.bmHeight };
		if (IntersectRect(&intersectRect, /*겹치는 영역 RECT*/
			&g_rectMouse, /*영역1 RECT*/
			&bitmapRect /*영역2 RECT*/
		) != 0) {
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
			HBRUSH hOlfBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Rectangle(hdc, g_rectMouse.left, g_rectMouse.top, g_rectMouse.right, g_rectMouse.bottom);
			DeleteObject(SelectObject(hdc, hOlfBrush));
		}
		// ===== IntersectRect() ===== end


		char szRectMouse[MAX_LOADSTRING] = {};
		sprintf_s(szRectMouse, "rectMouse : %d, %d, %d, %d", g_rectMouse.left, g_rectMouse.top, g_rectMouse.right, g_rectMouse.bottom);
		TextOut(hdc, 0, 140, szRectMouse, strnlen_s(szRectMouse, MAX_LOADSTRING));

		EndPaint(hDlg, &ps);

		DeleteObject(hBitmap);
		DeleteDC(hMemDC);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		// 멀티 모니터에서 부정확한 좌표를 반환하기 때문에 GET_X_LPARAM, GET_Y_LPARAM를 사용
		//rect.left = LOWORD(lParam);
		//rect.top = HIWORD(lParam);
		g_rectMouse.left = GET_X_LPARAM(lParam);
		g_rectMouse.top = GET_Y_LPARAM(lParam);

	}
	break;

	case WM_MOUSEMOVE:
	{
		HDC hdc = GetDC(hDlg);

		POINT ptMouse = {};
		ptMouse.x = GET_X_LPARAM(lParam);
		ptMouse.y = GET_Y_LPARAM(lParam);

		char szRectMouse[MAX_LOADSTRING] = {};
		sprintf_s(szRectMouse, "%d, %d", ptMouse.x, ptMouse.y);
		TextOut(hdc, 0, 100, szRectMouse, strnlen_s(szRectMouse, MAX_LOADSTRING));

		ReleaseDC(hDlg, hdc);

		if (wParam == MK_LBUTTON) {
			g_rectMouse.right = GET_X_LPARAM(lParam);
			g_rectMouse.bottom = GET_Y_LPARAM(lParam);
			InvalidateRect(hDlg, nullptr, true);
		}
	}
	break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
		case IDCANCEL:
			DestroyWindow(hDlg);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return false;
}
