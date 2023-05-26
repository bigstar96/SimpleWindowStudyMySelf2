#include "BitmapExample.h"

HRESULT BitmapExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackBuffer = std::make_unique<UINT8[]>(
		BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT
	);

	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				D2D1_ALPHA_MODE_IGNORE
			)
		),
		mspFrameBitmap.GetAddressOf()
	);

	ThrowIfFailed(hr);

	return S_OK;
}

void BitmapExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightPink));

	//FillRectToBuffer(100, 100, 100, 100, D2D1::ColorF::Blue);
	FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Red);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(0, 1, 0, 0.5f));


	PresentBuffer();

	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapExample::ClearBuffer(D2D1::ColorF color)
{
	UINT8* pCurrent = &mspBackBuffer[0];
	for (int count = 0; count < BITMAP_WIDTH * BITMAP_HEIGHT; ++count)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

		pCurrent += BITMAP_BYTECOUNT;
	}
}

void BitmapExample::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(
		nullptr,
		&mspBackBuffer[0],
		BITMAP_WIDTH * BITMAP_BYTECOUNT
	);
}

void BitmapExample::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;
	int index = pitch * y + x * BITMAP_BYTECOUNT;

	float inverse = 1.0f - color.a;
	UINT8 red2 = static_cast<UINT8>(color.r * 255);
	UINT8 green2 = static_cast<UINT8>(color.g * 255);
	UINT8 blue2 = static_cast<UINT8>(color.b * 255);

	/*mspBackBuffer[index] = static_cast<UINT8>(color.r * 255);
	mspBackBuffer[index + 1] = static_cast<UINT8>(color.g * 255);
	mspBackBuffer[index + 2] = static_cast<UINT8>(color.b * 255);
	mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);*/

	mspBackBuffer[index] = static_cast<UINT8>(mspBackBuffer[index] * inverse + red2 * color.a);
	mspBackBuffer[index + 1] = static_cast<UINT8>(mspBackBuffer[index + 1] * inverse + green2 * color.a);
	mspBackBuffer[index + 2] = static_cast<UINT8>(mspBackBuffer[index + 2] * inverse + blue2 * color.a);
	mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);
}

void BitmapExample::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			DrawPixelToBuffer(x + left, y + top, color);
		}
	}
}

