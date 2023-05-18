#include <Windows.h>

const wchar_t gClassName[] = L"MyWindowClass";

LRESULT CALLBACK WindowProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	HWND hWnd;
	WNDCLASSEX wc;

	// 1단계 - 윈도우 클래스 등록
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(
			nullptr, L"Failed To register window class!", L"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	RECT wr = { 0,0,640,480 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// 2단계 - 윈도우 생성
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Hello Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == nullptr)
	{
		MessageBox(
			nullptr, L"Failed to create window class!", L"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	// 3단계 - 윈도우 메시지 처리
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);
	

	EndPaint(hwnd, &ps);
}

// 4단계 - 윈도우 프로시져 작성
LRESULT CALLBACK WindowProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}