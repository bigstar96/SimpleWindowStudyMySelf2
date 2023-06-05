#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Data/bug1_1.png")
{
	mIsDeleted = false;
	mRotation = 0.0f;
	// mDirection = Direction::UP;
	mSteps = 0.0f;

	RECT rct;
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));
}

void Bug::Draw()
{
	/*
	if (mSteps++ > 5.0f)
	{
		mSteps = 0.0f;

		// 방향전환
		int dir = static_cast<int>(mDirection);
		int dirCount = static_cast<int>(Direction::COUNT);

		dir += (1 - rand() % 3);
		dir = (dir + dirCount) % dirCount;

		mDirection = static_cast<Direction>(dir);
	}

	switch (mDirection)
	{
	case Bug::Direction::UP:
		mY--;
		break;

	case Bug::Direction::UP_RIGHT:
		mY--;
		mX++;
		break;

	case Bug::Direction::RIGHT:
		mX++;
		break;

	case Bug::Direction::DOWN_RIGHT:
		mY++;
		mX++;
		break;

	case Bug::Direction::DOWN:
		mY++;
		break;

	case Bug::Direction::DOWN_LEFT:
		mY++;
		mX--;
		break;

	case Bug::Direction::LEFT:
		mX--;
		break;

	case Bug::Direction::UP_LEFT:
		mY--;
		mX--;
		break;

	}

	Actor::Draw();
	*/

	if (mSteps++ > 5.0f)
	{
		// 방향 전환 - 회전 각도
		mRotation += (1 - rand() % 3) * 45.0f;
		mSteps = 0.0f;
	}

	// 전진
	auto dir = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);
	mX += dir.x;
	mY += dir.y;

	auto pRT = mpFramework->GetRenderTarget();

	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rect
	{
		0, 0,
		static_cast<float>(0 + size.width),
		static_cast<float>(0 + size.height)
	};

	// 이동행렬
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	// 회전행렬
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation,
		D2D_POINT_2F{ size.width * 0.5f, size.height * 0.5f });

	pRT->SetTransform(matRotation * matTranslate);

	pRT->DrawBitmap(mpBitmap, rect, mOpacity);
}

bool Bug::IsClicked(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };

	D2D1_RECT_F rect{ mX, mY, static_cast<float>(mX + size.width), static_cast<float>(mY + size.height) };
	if (pt.x >= rect.left && pt.x <= rect.right &&
		pt.y >= rect.top && pt.y <= rect.bottom)
	{
		mIsDeleted = true;
		return true;
	}

	return false;
}
