#include <Windows.h>
#include <gdiplus.h>
#include <list>
#include "Card.h"
#include "GameLogic.h"

#pragma comment (lib, "Gdiplus.lib")

const wchar_t gClassName[] = L"MyWindowClass";

//std::list<solitaire::Card> myDeck;
solitaire::GameLogic gLogic;

LRESULT CALLBACK WindowProc(
	HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam
);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	/*myDeck.push_back(solitaire::Card(solitaire::Type::Bear, 0, 0));
	myDeck.push_back(solitaire::Card(solitaire::Type::Wolf, 120, 0));
	myDeck.push_back(solitaire::Card(solitaire::Type::Dragon, 240, 0));*/
	

	HWND hWnd;
	WNDCLASSEX wc;

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
			nullptr, L"Failed to register window class!", L"Error",
			MB_ICONEXCLAMATION | MB_OK
		);
		return 0;
	}

	RECT wr = { 0,0,1024,768 };

	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU, FALSE);
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Solitaire Game",
		WS_OVERLAPPED | WS_SYSMENU,
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

	gLogic.Init(hWnd);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	gLogic.Release();

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	Gdiplus::Graphics graphics(hdc);

	/*for (auto& card : myDeck)
	{
		card.Draw(graphics);
	}*/

	gLogic.Draw(graphics);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(
	HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam
)
{
	switch (message)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		break;

	case WM_LBUTTONUP:
		gLogic.OnClick(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}