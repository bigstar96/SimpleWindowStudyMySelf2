#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework*	mpFramework;
	ID2D1Bitmap*	mpBitmap;

	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);

	virtual ~Actor();

	virtual void Draw();

private:
	//HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);
};

