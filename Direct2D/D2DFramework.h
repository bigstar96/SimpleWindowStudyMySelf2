#pragma once

#include <d2d1.h>
#include <wrl/client.h>

class D2DFramework
{
protected:
	Microsoft::WRL::ComPtr<ID2D1Factory>			mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>	mspRenderTarget;
	
public:
	virtual HRESULT Init(HWND hwnd);
	virtual void Release();
	virtual void Render();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error = 0, LPCWSTR title = L"Error");
};

