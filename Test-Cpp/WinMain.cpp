#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

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
	//wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)); // IDI_APPLICATION : �ü���� �̸� ���ǵ� �⺻ ������

	// �⺻ ���� ������ ����
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wcex.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));

	// �⺻ Ŀ�� ����, OS���� �̸� ������ Ŀ���� ����� �� �ִ�.
	// IDC : ID for an cursor
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

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
	wcex.lpszMenuName = NULL;


	// 2. Window Class ����ü ���
	RegisterClassEx(&wcex);


	// 3. Window ����
	HWND hWnd = CreateWindow(
		strClassName, // LPCSTR lpClassName : Window Class �̸�
		TEXT("Hello World"), // LPCSTR lpWindowName : ������ Ÿ��Ʋ �̸�
		WS_OVERLAPPEDWINDOW, // DWORD dwStyle : ������ ��Ÿ��, dw(DWORD)
		CW_USEDEFAULT, 0, // int X, int Y : ������ x, y ��ǥ, CW(Create Window), CW_USEDEFAULT : �ü���� ���� �⺻���� ����Ѵ�.
		CW_USEDEFAULT, 0, // int nWidth, int nHeight : ������ ����, ���� ũ��(Ŭ���̾�Ʈ ���� ����)
		nullptr, // HWND hWndParent : �θ� ������ �ڵ�
		nullptr, // HMENU hMenu : �޴� �ڵ�
		hInstance, // HINSTANCE hInstance : ���� ���α׷� �ν��Ͻ�
		nullptr // LPVOID lpParam : ���� ������ ����, ���������� ������� �ʴ´�.
	);


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
	HDC hdc;

	switch (message)
	{
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