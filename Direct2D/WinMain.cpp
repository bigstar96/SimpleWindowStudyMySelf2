#include <Windows.h>

//#include <d2d1.h>
//#include <math.h>
//#pragma comment	(lib, "d2d1.lib")

#include "D2DFramework.h"

const wchar_t gClassName[]{ L"MyWindowClass" };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// 0. D2D 전역 변수들
//ID2D1Factory* gpD2DFactory{};
//ID2D1HwndRenderTarget* gpRenderTarget{};
//ID2D1SolidColorBrush* gpBrush{};
//ID2D1RadialGradientBrush* gpRadialBrush{};

//void OnPaint(HWND hwnd);

D2DFramework myFramework;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
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
		MessageBox(NULL, L"Failed To Register", L"Error", MB_OK);
		return 0;
	}

	RECT rc{ 0,0,1024,768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Direct2D",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Failed to create", L"Error", MB_OK);
		return 0;
	}

	myFramework.Init(hwnd);

	/*
	// 1. D2D Factory 만들기
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create D2D Factory!", L"Error", MB_OK);
		return 0;
	}

	// 2. RenderTarget 생성
	hr = gpD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
		),
		&gpRenderTarget
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create D2D Render Target!", L"Error", MB_OK);
		return 0;
	}

	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0x9ACD32, 1.0f)),
		&gpBrush
	);

	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];

	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1);
	gradientStops[1].position = 1.0f;

	hr = gpRenderTarget->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);

	if (SUCCEEDED(hr))
	{
		hr = gpRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(50, 150),
				D2D1::Point2F(0, 0),
				50,
				50
			),
			pGradientStops,
			&gpRadialBrush
		);
	}
	*/

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	/*MSG msg;
	while (GetMessage(&msg, NULL, 0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			//OnPaint(hwnd);
			myFramework.Render();
		}
	}

	/*
	// 4. 해제
	if (gpBrush)
	{
		gpBrush->Release();
		gpBrush = nullptr;
	}
	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}
	if (gpD2DFactory)
	{
		gpD2DFactory->Release();
		gpD2DFactory = nullptr;
	}

	if (pGradientStops != nullptr)
	{
		pGradientStops->Release();
		pGradientStops = nullptr;
	}
	if (gpRadialBrush)
	{
		gpRadialBrush->Release();
		gpRadialBrush = nullptr;
	}
	if (gpBrush)
	{
		gpBrush->Release();
		gpBrush = nullptr;
	}
	*/

	myFramework.Release();

	return static_cast<int>(msg.wParam);
}

/*
void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);

	// 3. 그리기
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	//gpBrush->SetOpacity(1.0f);
	//gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Aquamarine));
	//gpRenderTarget->FillRectangle(
	//	D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
	//	gpBrush
	//);

	//gpBrush->SetOpacity(0.5f);
	//gpBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
	//gpRenderTarget->FillRectangle(
	//	D2D1::RectF(50.0f, 50.0f, 150.0f, 150.0f),
	//	gpBrush
	//);

	gpRenderTarget->FillRectangle(
		D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f),
		gpBrush
	);

	static float fAngle = 0.0;

	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(75.0f + sinf(fAngle) * 25.0f, 150.0f), 50.0f, 50.0f), 
		gpRadialBrush
	);

	fAngle += 0.05f;

	gpRenderTarget->EndDraw();



	EndPaint(hwnd, &ps);
}
*/


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		/*
	case WM_PAINT:
		OnPaint(hwnd);
		break;
		*/
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

