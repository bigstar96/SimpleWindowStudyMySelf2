#include "Actor.h"

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework{ pFramework },
	mX{}, mY{}, mOpacity{ 1.0f }
{
	LoadWICImage(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) : Actor(pFramework, filename)
{
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor()
{
	mspBitmap.Reset();
}

void Actor::Draw()
{
	Draw(mX, mY, mOpacity);
}

HRESULT Actor::LoadWICImage(LPCWSTR filename)
{
	// decoder
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;
	
	auto pWICFactory{ mpFramework->GetWICFactory() };
	if (pWICFactory == nullptr)
	{
		return E_FAIL;
	}

	auto pRT{ mpFramework->GetRenderTarget() };
	if (pRT == nullptr)
	{
		return E_FAIL;
	}

	hr = pWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// frame
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	hr = bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// converter
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = pWICFactory->CreateFormatConverter(converter.GetAddressOf());
	ThrowIfFailed(hr);

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);
	
	// bitmap
	hr = pRT->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}

void Actor::Draw(float x, float y, float opacity)
{
	auto pRT = mpFramework->GetRenderTarget();
	auto size = mspBitmap->GetPixelSize();
	D2D1_RECT_F rect{ x, y, 
		static_cast<float>(x + size.width), 
		static_cast<float>(y + size.height)
	};


	pRT->DrawBitmap(
		mspBitmap.Get(),
		rect,
		opacity
	);
}
