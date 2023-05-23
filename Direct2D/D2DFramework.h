#pragma once

#include <d2d1.h>
#include <wrl/client.h>
#include <exception>
#include <sstream>

class com_exception : public std::exception
{
private:
	HRESULT mResult;

public:
	com_exception(HRESULT hr) : mResult{ hr }
	{
	}

	virtual const char* what() const override
	{
		std::stringstream ss;
		ss << "Failure with Code : " <<
			mResult << std::endl;

		return ss.str().c_str();
	}
};

inline void ThrowIfFailed(HRESULT hr);

class D2DFramework
{
private:
	const LPCWSTR WINDOW_CLASSNAME{ L"MyD2DWindowClass" };

protected:
	HWND mHwnd;
	Microsoft::WRL::ComPtr<ID2D1Factory>			mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>	mspRenderTarget;
	
protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT InitD2D(HWND hwnd);
	virtual HRESULT	CreateDeviceResources();

public:
	virtual void Initialize(HINSTANCE hInstance, LPCWSTR title = L"D2DFramework",
		UINT width = 1024, UINT height = 768);
	virtual void Release();
	virtual void Render();
	virtual int GameLoop();

	void ShowErrorMsg(LPCWSTR msg, HRESULT error = 0, LPCWSTR title = L"Error");

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

