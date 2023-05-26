#pragma once
#include "D2DFramework.h"
class ImageExaple : public D2DFramework
{
	Microsoft::WRL::ComPtr<ID2D1Bitmap>	mspBitmap{};

public:
	virtual HRESULT Initialize(
		HINSTANCE hInstance, 
		LPCWSTR title = L"BMP File Example",
		UINT width = 1024,
		UINT height = 768
	) override;


	void Render() override;

	HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};

