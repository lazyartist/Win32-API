#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "resource.h"

HINSTANCE g_hInstance;

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
	HDC hdc;

	switch (message)
	{
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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}