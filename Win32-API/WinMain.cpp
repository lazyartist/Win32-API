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


	// 0. 리소스 파일에서 스트링 가져오기
	TCHAR szWindowName[100];
	LoadString(hInstance, IDS_TITLE, szWindowName, 100);

	// 1. Window Class 구조체 설정

	// Window Class 구조체 생성
	WNDCLASSEX wcex;

	// WNDCLASSEX의 크기를 입력
	// cb : count of bytes
	wcex.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 출력 형태 지정. 가로, 세로 사이즈가 변하면 전체를 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메시지 처리에 사용될 함수
	wcex.lpfnWndProc = WndProc;

	// 동일한 'Window Class'를 사용하는 Window들이 공유할 수 있는 메모리 크기를 설정
	// 40바이트까지 제한.
	// SetClassWord, SetClassLong, GetClassWord, GetClassLong 함수를 사용하여 사용
	// 보안에 취약하고 크기도 제한적이라서 거의 사용하지 않음.
	wcex.cbClsExtra = 0;

	// 이 'Window Class'를 사용하는 Window를 위한 개별 메모리를 추가. 동일한 'Window Class'를 사용해도 공유되지 않는다.
	// 40바이트까지 제한
	// SetWindowLong, GetWindowLong 함수를 사용하여 사용
	// 거의 사용하지 않음.
	wcex.cbWndExtra = 0;

	// 윈도우 인스턴스
	wcex.hInstance = hInstance;

	// 기본 아이콘 지정, OS에서 미리 정의한 아이콘을 사용할 수 있다.
	// IDI : ID for an icon
	// 방법 1. 리소스의 이름을 문자열 포인터로 전달
	wcex.hIcon = LoadIcon(NULL, "resourceName");
	// 방법 2. Resource.h에 define된 리소스 아이디를 MAKEINTRESOURCE를 이용해 문자열 포인터 형식으로 변경
	// 리소스 아이디로 변경된 문자열 포인터는 유효하지 않지만 OS는 이를 ID값으로 사용하여 문자열을 찾는다.(?)
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // IDI_APPLICATION : 운영체제에 미리 정의된 기본 아이콘
	// 방법 3. 미리정의된 아이콘이 아닌 경우 MAKEINTRESOURCE 매크로를 사용해 정수 아이디값을 문자로 바꿔준다.
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); // IDI_APPLICATION : 운영체제에 미리 정의된 기본 아이콘

	// 기본 작은 아이콘 지정
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wcex.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

	// 기본 커서 지정, OS에서 미리 정의한 커서를 사용할 수 있다.
	// IDC : ID for an cursor
	// 방법 1. 운영체제의 것을 사용한다. instance를 NULL로 준다.
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	//wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	// 방법 2. 임의의 커서를 사용한다.
	//wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));


	// 윈도우의 배경 색을 지정
	// 방법 1. CreateSolidBrush()로 브러시를 직접 생성하여 할당. 
	// 프로그램 종료 시 반드시 해제해야한다.
	HBRUSH brushBg = CreateSolidBrush(RGB(0, 0, 0));
	wcex.hbrBackground = brushBg;
	// 방법  2. OS가 미리 설정한 상숫값 사용
	// COLOR_WINDOW : 회색
	// COLOR_WINDOW + 1 : 흰색
	// COLOR_WINDOW + 2 : 어두운 회색
	// COLOR_WINDOW + 3 : 검은색
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	// 방법 3. OS가 미리 만들어 놓은 Stock Resource를 가져다 사용
	// OS가 사용빈도가 높은 자원을 미리 만들어 놓는데 이 자원을 Stock Resource 또는 Stock Object라 한다.
	// 이 자원중 WHITE_BRUSH에 해당하는 자원의 핸들값을 얻어 배경색으로 지정한다.
	// 반환값이 HGDIOBJ이므로 형변환하여 사용한다.
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// Window Class의 이름을 지정하고 이 이름으로 Window Class를 구분하기 때문에 다른 Window Class와 중복되지 않게 잘 지정한다.
	// 대소문자를 구분하지 않는다. 
	LPCTSTR strClassName = TEXT("Win32");
	wcex.lpszClassName = strClassName;

	// 메뉴 이름
	//wcex.lpszMenuName = MAKEINTRESOURCE(106);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);


	// 2. Window Class 구조체 등록
	RegisterClassEx(&wcex);


	// 3. Window 생성
	HWND hWnd = CreateWindow(
		strClassName, // LPCSTR lpClassName : Window Class 이름
		szWindowName, // LPCSTR lpWindowName : 윈도우 타이틀 이름
		//TEXT("Hello World"), // LPCSTR lpWindowName : 윈도우 타이틀 이름
		WS_OVERLAPPEDWINDOW, // DWORD dwStyle : 윈도우 스타일, dw(DWORD)
		CW_USEDEFAULT, 0, // int X, int Y : 윈도우 x, y 좌표, CW(Create Window), CW_USEDEFAULT : 운영체제가 정한 기본값을 사용한다.
		CW_USEDEFAULT, 11, // int nWidth, int nHeight : 윈도우 가로, 세로 크기(클라이언트 영역 포함)
		nullptr, // HWND hWndParent : 부모 윈도우 핸들
		nullptr, // HMENU hMenu : 메뉴 핸들
		hInstance, // HINSTANCE hInstance : 응용 프로그램 인스턴스
		nullptr // LPVOID lpParam : 생성 윈도우 정보, 여유분으로 사용하지 않는다.
	);
	g_hMainWnd = hWnd;


	// 4. 윈도우 출력
	// 방법 1. 프로그램 실행 시 넘어온 ShowWindow Commands를 사용한다.
	ShowWindow(hWnd, nCmdShow);
	// 방법 2. 미리 정의된 ShowWindow Commands를 사용한다.
	ShowWindow(hWnd, SW_SHOW);
	// WM_PAINT 메시지를 발생시켜 윈도우를 그린다.
	UpdateWindow(hWnd);


	// 5. 메시지 루프
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
	case WM_CREATE: // 윈도우가 처음 생성됐을 때 발생
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
		// Pen 생성
		HPEN hPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
		// DC에 펜 지정
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
		// 비트맵 헤더
		BITMAP resBitmapHeader; // 리소스 비트맵 헤더
		BITMAP fileBitmapHeader; // 파일 비트맵 헤더

		// 리소스의 비트맵 불러오기
		HBITMAP hResBitmap = LoadBitmap(g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		// 파일의 비트맵 불러오기, 파일은 프로젝트 폴더가 기준이다.(프로젝트 폴더 밖의 Debug 폴더에 넣지 않는다)
		HBITMAP hFileBitmap = (HBITMAP)LoadImage(NULL, TEXT("zzal.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// 비트맵 헤더를 읽어들임
		GetObject(hResBitmap, sizeof(BITMAP), &resBitmapHeader);
		GetObject(hFileBitmap, sizeof(BITMAP), &fileBitmapHeader);

		// Memory DC 생성
		HDC hResMemDC = CreateCompatibleDC(hdc);
		HDC hFileMemDC = CreateCompatibleDC(hdc);

		// Memory DC에 비트맵 선택
		SelectObject(hResMemDC, hResBitmap);
		SelectObject(hFileMemDC, hFileBitmap);

		// Memory DC를 화면 DC에 복사
		BitBlt(hdc, 300, 300, resBitmapHeader.bmWidth, resBitmapHeader.bmHeight, hResMemDC, 0, 0, SRCCOPY);

		//BitBlt(hdc, 300 + resBitmapHeader.bmWidth, 300, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, hFileMemDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, 300 + resBitmapHeader.bmWidth, 300, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, hFileMemDC, 0, 0, fileBitmapHeader.bmWidth, fileBitmapHeader.bmHeight, RGB(231, 223, 220));


		// 생성했던 비트맵과 Memory DC를 제거
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
		// 드래그하여 사각형 그리기 방법 1.
		if (g_bDragging) {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			HDC hdc = GetDC(hWnd);
			Rectangle(hdc, g_dragStartPosition.x, g_dragStartPosition.y, x, y);
			ReleaseDC(hWnd, hdc);
		}

		// 드래그하여 사각형 그리기 방법 2.
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

		// 모델리스 다이얼로그 생성
		// IsWindow() 함수로 현재 유효한 윈도우 핸들인지 확인한다.
		if (IsWindow(g_hModelessDialog) == FALSE) {
			// CreateDialog()는 생성한 다이얼로그의 핸들을 바로 반환한다.
			g_hModelessDialog = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, DialogProc_Modeless);

			// 다이얼로그를 생성했지만 아직 보이지 않기 때문에 화면에 보이도록 한다.
			// 디얼로그에 WS_VISIBLE 스타일을 주었다면 
			// ShowWindow() 함수를 호출하지 않아도 되지만
			// 디폴트가 WS_VISIBLE 스타일이 아니므로 호출한다.
			ShowWindow(g_hModelessDialog, SW_SHOW);
		}
		break;
	case WM_MBUTTONDOWN:
	{
		// 모달 다이얼로그 박스 생성
		// DialogBox의 반환값을 보고 성공/취소를 판단하여 처리한다.
		// 반환값을 EndDialog의 두번째 파라미터로 넘기는 값이 반환된다.
		if (DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc) != 0) {
			InvalidateRect(hWnd, NULL, TRUE);
		};
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0); // GetMessage() 함수가 0을 반환하게 된다.
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

// 모달 다이얼로그 프로시저
INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		// 다이얼로그 초기화 시 각 컨트롤러에 초기값을 넣어준다.
		SetDlgItemInt(hWnd, IDC_X, g_nDialogX, FALSE);
		SetDlgItemInt(hWnd, IDC_Y, g_nDialogY, FALSE);
		SetDlgItemText(hWnd, IDC_STR, g_szDialogStr);
		return true;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			// 작업이 끝나고 다이얼로그창을 성공적으로 닫을 때 컨트롤러에 있는 값을 넣어준다.
			g_nDialogX = GetDlgItemInt(hWnd, IDC_X, NULL, FALSE);
			g_nDialogY = GetDlgItemInt(hWnd, IDC_Y, NULL, FALSE);
			GetDlgItemText(hWnd, IDC_STR, g_szDialogStr, 100);
			EndDialog(hWnd, 1); // DialogBox()의 리턴값이 1이 된다.
			break;
		case IDCANCEL:
			// 다이얼로그 작업을 취소할 경우 그대로 나간다.
			EndDialog(hWnd, FALSE); // DialogBox()의 리턴값이 0이 된다.
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

// 모델리스 다이얼로그 프로시저
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
			InvalidateRect(g_hMainWnd, NULL, TRUE); // 데이터 변경을 메인윈도우의 화면에 반영
			break;
		case ID_CLOSE:
			DestroyWindow(g_hModelessDialog); // CreateDialog()로 열었으므로 DestroyWindow()로 닫는다.
			//EndDialog(hWnd, FALSE); // 닫히긴하지만 다시 열리지 않는다.
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