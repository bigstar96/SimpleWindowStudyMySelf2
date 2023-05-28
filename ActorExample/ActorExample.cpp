#include "ActorExample.h"

HRESULT ActorExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;
	hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr);

	// �ʱ�ȭ�� �ʿ��� ��
	mspBackground = std::make_unique<Actor>(this, L"Data/back1_1024.png", 0.0f, 0.0f);
	for (int i = 0; i < 20; ++i)
	{
		mspBug.push_back(std::make_unique<Actor>(this, L"Data/bug1_1.png", i * 20.0f , i * 10.0f));
	}
	

	return S_OK;
}

void ActorExample::Release()
{
	// ����� �����޸� ����
	mspBackground.reset();
	
	for (auto& e : mspBug)
	{
		e.reset();
	}

	D2DFramework::Release();
}

void ActorExample::Render()
{
	HRESULT hr;

	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	// �׸� ��
	mspBackground->Draw();
	for (auto& e : mspBug)
	{
		e->Draw();
	}


	hr = mspRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
