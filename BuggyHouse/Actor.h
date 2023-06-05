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
	Actor() = default;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);

	virtual ~Actor();

	virtual void Draw();

private:
	//HRESULT LoadWICImage(LPCWSTR filename);
	void Draw(float x, float y, float opacity = 1.0f);

public:
	inline D2D_VECTOR_2F GetPosition() const
	{
		return D2D_VECTOR_2F{ mX, mY };
	}
	inline void SetPosition(D2D_VECTOR_2F& position)
	{
		mX = position.x;
		mY = position.y;
	}
	inline void SetPosition(float x, float y)
	{
		mX = x;
		mY = y;
	}
};

