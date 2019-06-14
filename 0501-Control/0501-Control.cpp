#include "Windows.h"

HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hInst = hInstance;

	// 윈도우 클래스 등록
	WNDCLASSEX wcex = { 0, };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEST);
	wcex.lpszClassName = "MyButton";
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	// 윈도우 클래스를 기반으로 윈도우 생성
	HWND hWnd = CreateWindowEx(0, "MyButton", "Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr) {
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 메시지 루프
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
	{
		// Button 컨트롤 생성
		// 생성한 컨트롤 핸들을 반환한다. 
		HWND hButton = CreateWindowEx(0,
			// 만들고자 하는 윈도우 클래스
			"button",

			// 윈도우의 캡션, 버튼일 경우 버튼 위에 나타난다.
			"label",

			// WS_CHILD : 컨트롤은 반드시 자식 윈도우
			// WS_VISIBLE : ShowWindow 함수를 호출하지 않아도 화면에 나타냄
			// BS_PUSHBUTTON : 버튼의 스타일 값
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,

			// 위치와 크기
			20,
			20,
			50,
			30,

			// 부모 윈도우
			hWnd,

			// 윈도우에서 사용할 메뉴의 핸들이지만 컨트롤의 경우에는 메뉴를 가지지 않으므로
			// 이 인수는 컨트롤의 ID를 지정하는 용도로 사용한다.
			// 자식 윈도우끼리만 중복되지 않으면 된다.
			(HMENU)10,

			// 인스턴스 핸들
			g_hInst,

			// MDI에서 사용하는 구조체
			nullptr);
		// Button 컨트롤 생성 end

		// Static 컨트롤 생성
		HWND hStatkc = CreateWindowEx(0, "static", "static controll text", WS_CHILD | WS_VISIBLE, 20, 50, 200, 50, hWnd, (HMENU)20, g_hInst, nullptr);
		// Static 컨트롤 생성 end
	}

	break;
	case WM_COMMAND: // 사용자로부터의 명령
	{
		// 컨트롤 ID, 메뉴 ID, 액셀레이터 ID 등이 모두 LOWORD(wParam)로 전달되므로
		// 2바이트, 즉 0~65535 사이의 값중에서 중복되지 않는 값이어야 한다.
		switch (LOWORD(wParam))
		{
		case 10:
			// 버튼 컨트롤 클릭시 전달되는 정보
			// HIWORD(wParam) : 통지 코드, 버튼의 경우 항상 BN_CLICKED(클릭됨)
			// LOWORD(wParam) : 컨트롤의 ID
			// lParam : 메시지를 보낸 차일드 윈도우의 메시지 핸들
			if (HIWORD(wParam) == BN_CLICKED) {
				MessageBox(hWnd, "button button", "button caption", MB_OK);
			}
			break;
		default:
			break;
		}
	}
	break;
	case WM_CLOSE: // 윈도우가 닫힌다.
	case WM_DESTROY: // 윈도우가 제거됐다.
		PostQuitMessage(0); // 프로그램을 종료한다.
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // DefWindowProc() 반환값을 리턴한다.
		break;
	}

	// false(0)을 리턴하면 메시지를 제대로 처리하지 않았음을 나타낸다.
	// 윈도우 생성중(CreateWindow()함수 내부)에 false를 리턴하면 윈도우가 생성되지 않기 때문에 
	// 처리하는 메시지가 없어도 DefWindowProc를 통해 0이 아닌 값을 리턴하도록 해야한다.
	// 윈도우 생성 후에는 메시지가 제대로 처리되지 않았음을 나타낼뿐 프로그램이 종료되지는 않는다.
	return 0; 
}