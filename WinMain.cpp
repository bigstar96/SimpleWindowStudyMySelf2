#include <Windows.h>

#include <sstream>

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

	// 2단계 - 윈도우 생성
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Hello Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
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
	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(hwnd, &ps);

	HPEN bluePen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	HPEN oldPen = (HPEN)SelectObject(hdc, bluePen);
	MoveToEx(hdc, 0, 110, nullptr);
	LineTo(hdc, 100, 110);
	DeleteObject(bluePen);
	SelectObject(hdc, oldPen);

	HBRUSH hatchBrush = CreateHatchBrush(HS_CROSS, RGB(255, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hatchBrush);
	Rectangle(hdc, 0, 0, 100, 100);
	DeleteObject(hatchBrush);
	SelectObject(hdc, oldBrush);

	EndPaint(hwnd, &ps);
}

// 4단계 - 윈도우 프로시져 작성
LRESULT CALLBACK WindowProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_ERASEBKGND:
	//	break;
	
	case WM_PAINT:
	{
		OnPaint(hWnd);
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}