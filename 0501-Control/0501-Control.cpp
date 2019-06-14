#include "Windows.h"

HINSTANCE g_hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT_PTR APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	g_hInst = hInstance;

	// ������ Ŭ���� ���
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

	// ������ Ŭ������ ������� ������ ����
	HWND hWnd = CreateWindowEx(0, "MyButton", "Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
	if (hWnd == nullptr) {
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// �޽��� ����
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
		// Button ��Ʈ�� ����
		// ������ ��Ʈ�� �ڵ��� ��ȯ�Ѵ�. 
		HWND hButton = CreateWindowEx(0,
			// ������� �ϴ� ������ Ŭ����
			"button",

			// �������� ĸ��, ��ư�� ��� ��ư ���� ��Ÿ����.
			"label",

			// WS_CHILD : ��Ʈ���� �ݵ�� �ڽ� ������
			// WS_VISIBLE : ShowWindow �Լ��� ȣ������ �ʾƵ� ȭ�鿡 ��Ÿ��
			// BS_PUSHBUTTON : ��ư�� ��Ÿ�� ��
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,

			// ��ġ�� ũ��
			20,
			20,
			50,
			30,

			// �θ� ������
			hWnd,

			// �����쿡�� ����� �޴��� �ڵ������� ��Ʈ���� ��쿡�� �޴��� ������ �����Ƿ�
			// �� �μ��� ��Ʈ���� ID�� �����ϴ� �뵵�� ����Ѵ�.
			// �ڽ� �����쳢���� �ߺ����� ������ �ȴ�.
			(HMENU)10,

			// �ν��Ͻ� �ڵ�
			g_hInst,

			// MDI���� ����ϴ� ����ü
			nullptr);
		// Button ��Ʈ�� ���� end

		// Static ��Ʈ�� ����
		HWND hStatkc = CreateWindowEx(0, "static", "static controll text", WS_CHILD | WS_VISIBLE, 20, 50, 200, 50, hWnd, (HMENU)20, g_hInst, nullptr);
		// Static ��Ʈ�� ���� end
	}

	break;
	case WM_COMMAND: // ����ڷκ����� ���
	{
		// ��Ʈ�� ID, �޴� ID, �׼������� ID ���� ��� LOWORD(wParam)�� ���޵ǹǷ�
		// 2����Ʈ, �� 0~65535 ������ ���߿��� �ߺ����� �ʴ� ���̾�� �Ѵ�.
		switch (LOWORD(wParam))
		{
		case 10:
			// ��ư ��Ʈ�� Ŭ���� ���޵Ǵ� ����
			// HIWORD(wParam) : ���� �ڵ�, ��ư�� ��� �׻� BN_CLICKED(Ŭ����)
			// LOWORD(wParam) : ��Ʈ���� ID
			// lParam : �޽����� ���� ���ϵ� �������� �޽��� �ڵ�
			if (HIWORD(wParam) == BN_CLICKED) {
				MessageBox(hWnd, "button button", "button caption", MB_OK);
			}
			break;
		default:
			break;
		}
	}
	break;
	case WM_CLOSE: // �����찡 ������.
	case WM_DESTROY: // �����찡 ���ŵƴ�.
		PostQuitMessage(0); // ���α׷��� �����Ѵ�.
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam); // DefWindowProc() ��ȯ���� �����Ѵ�.
		break;
	}

	// false(0)�� �����ϸ� �޽����� ����� ó������ �ʾ����� ��Ÿ����.
	// ������ ������(CreateWindow()�Լ� ����)�� false�� �����ϸ� �����찡 �������� �ʱ� ������ 
	// ó���ϴ� �޽����� ��� DefWindowProc�� ���� 0�� �ƴ� ���� �����ϵ��� �ؾ��Ѵ�.
	// ������ ���� �Ŀ��� �޽����� ����� ó������ �ʾ����� ��Ÿ���� ���α׷��� ��������� �ʴ´�.
	return 0; 
}