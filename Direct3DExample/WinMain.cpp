#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D" };
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

Microsoft::WRL::ComPtr<ID3D11Device> gspDevice{};
Microsoft::WRL::ComPtr<ID3D11DeviceContext> gspDeviceContext{};
Microsoft::WRL::ComPtr<IDXGISwapChain> gspSwapChain{};

Microsoft::WRL::ComPtr<ID3D11Texture2D> gspRenderTarget{};
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> gspRenderTargetView{};
Microsoft::WRL::ComPtr<ID3D11Texture2D> gspDepthStencil{};
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> gspDepthStencilView{};

HWND gHwnd{};
HINSTANCE gInstance{};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitD3D();
void DestroyD3D();
void Render();

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	gInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = gClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to register window class!", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RECT wr{ 0,0, gScreenWidth, gScreenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	gHwnd = CreateWindowEx(
		NULL,
		gClassName,
		gTitle,
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

	if (gHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed to create window class", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(gHwnd, nShowCmd);

	SetForegroundWindow(gHwnd);
	SetFocus(gHwnd);

	UpdateWindow(gHwnd);

	InitD3D();

	MSG msg{};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
			// Game Loop
			Render();
		}
	}

	DestroyD3D();

	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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

void InitD3D()
{
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = gScreenWidth;
	scd.BufferDesc.Height = gScreenHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = gHwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(
		NULL,							// Adapter pointer
		D3D_DRIVER_TYPE_HARDWARE,		// 물리적인 하드웨어 (그래픽카드)
		NULL,							// 소프트웨어 렌더러 지정
		NULL,							// 옵션
		NULL,							// D3D_FEATURE_LEVEL 배열
		0,								// 배열 크기
		D3D11_SDK_VERSION,				// SDK
		&scd,							// DXGI_SWAP_CHAIN_DESC 구조체에 대한 *(pointer)
		gspSwapChain.ReleaseAndGetAddressOf(),
		gspDevice.ReleaseAndGetAddressOf(),
		NULL,
		gspDeviceContext.ReleaseAndGetAddressOf()
	);

	// 스왑체인(Front, Back) <- Back 그릴예정 <- RenderTarget을 BackBuffer로 지정
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.ReleaseAndGetAddressOf()));

	// resource -> resource view 연결
	gspDevice->CreateRenderTargetView(gspRenderTarget.Get(), nullptr, gspRenderTargetView.GetAddressOf());

	// Depth-Stencil Buffer
	CD3D11_TEXTURE2D_DESC t2d(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		gScreenWidth,
		gScreenHeight,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);


	gspDevice->CreateTexture2D(&t2d, nullptr, gspDepthStencil.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);
	gspDevice->CreateDepthStencilView(
		gspDepthStencil.Get(),
		&dsvd,
		gspDepthStencilView.ReleaseAndGetAddressOf()
	);

	// Output Merger 연결
	gspDeviceContext->OMSetRenderTargets(
		1,
		gspRenderTargetView.GetAddressOf(),
		gspDepthStencilView.Get()
	);

	// Viewport
	CD3D11_VIEWPORT viewport(
		0.0f, 0.0f,
		static_cast<float>(gScreenWidth),
		static_cast<float>(gScreenHeight)
	);
	gspDeviceContext->RSSetViewports(1, &viewport);
}

void DestroyD3D()
{
	gspSwapChain->SetFullscreenState(FALSE, nullptr);

	gspDepthStencilView.Reset();
	gspDepthStencil.Reset();
	gspRenderTargetView.Reset();
	gspRenderTarget.Reset();
	gspSwapChain.Reset();
	gspDeviceContext.Reset();
	gspDevice.Reset();


	DestroyWindow(gHwnd);
	UnregisterClass(gClassName, gInstance);
}

void Render()
{
	float clear_color[4]{ 0.0f,0.2f,0.4f,1.0f };

	gspDeviceContext->ClearRenderTargetView(gspRenderTargetView.Get(), clear_color);
	gspDeviceContext->ClearDepthStencilView(
		gspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	gspSwapChain->Present(0, 0);
}