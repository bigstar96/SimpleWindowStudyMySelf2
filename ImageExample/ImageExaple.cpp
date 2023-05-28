#include "ImageExaple.h"
#include <fstream>
#include <vector>

#pragma comment(lib, "WindowsCodecs.lib")

HRESULT ImageExaple::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = CoInitialize(nullptr);
	ThrowIfFailed(hr);

	//WICFactory ����
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);

	D2DFramework::Initialize(hInstance, title, width, height);

	//LoadBMP(L"Data/32.bmp", mspBitmap.GetAddressOf());
	LoadWICImage(L"Data/3.png", mspBitmap.GetAddressOf());

	return S_OK;
}

void ImageExaple::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::LightSalmon));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	mspRenderTarget->EndDraw();
}

void ImageExaple::Release()
{
	D2DFramework::Release();

	mspBitmap.Reset();
	mspWICFactory.Reset();

	CoUninitialize();
}

HRESULT ImageExaple::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. ���� ����
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// 2. BITMAPFILEHEADER �б�
	BITMAPFILEHEADER bfh;
	file.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));

	// 3. BITMAPINFOHEADER �б� 
	BITMAPINFOHEADER bih;
	file.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	if (bfh.bfType != 0x4D42)
	{
		return E_FAIL;
	}
	if (bih.biBitCount != 32)
	{
		return E_FAIL;
	}

	// 4. bfOffset ��ŭ �ǳʶٱ�
	file.seekg(bfh.bfOffBits);
	
	// 5. �ȼ� �迭 �б�
	std::vector<char> pPixels(bih.biSizeImage);

	//file.read(&pPixels[0], bih.biSizeImage);

	int pitch = bih.biWidth * (bih.biBitCount / 8);

	/*for (int y = bih.biHeight - 1; y >= 0; --y)
	{
		file.read(&pPixels[y*pitch], pitch);
	}*/

	int index{};
	for (int y = bih.biHeight - 1; y >= 0; --y)
	{
		index = y * pitch;
		for (int x = 0; x < bih.biWidth; ++x)
		{
			char r{}, g{}, b{}, a{};

			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (static_cast<unsigned char>(r) == 30 && 
				static_cast<unsigned char>(g) == 199 &&
				static_cast<unsigned char>(b) == 250)
			{
				r = g = b = a = 0;
			}

			pPixels[index++] = b;
			pPixels[index++] = g;
			pPixels[index++] = r;
			pPixels[index++] = a;
		}
	}

	file.close();

	//int pitch = bih.biWidth * (bih.biBitCount / 8);

	// 6. 5�� �����ͷ� BITMAP �����
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(bih.biWidth, bih.biHeight),
		D2D1::BitmapProperties(
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		),
		ppBitmap
	);

	ThrowIfFailed(hr);

	(*ppBitmap)->CopyFromMemory(nullptr, &pPixels[0], pitch);

	return S_OK;
}

HRESULT ImageExaple::LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// 1. Decoder ����
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;
	hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 2. Decoder���� Frame ȹ��
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	bitmapDecoder->GetFrame(0, frame.GetAddressOf());
	ThrowIfFailed(hr);

	// 3. Converter�� ����ؼ� ���ϴ� �������� ��ȯ
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	hr = mspWICFactory->CreateFormatConverter(converter.GetAddressOf());
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

	// 4. Converter���� �ȼ� ������ -> ��Ʈ�� ����
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		mspBitmap.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

	return S_OK;
}
