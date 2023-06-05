#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) : Actor(pFramework, L"Data/bug1_1.png")
{
	RECT rct;
	GetClientRect(pFramework->GetWindowHandle(), &rct);

	mX = static_cast<float>(rand() % (rct.right - rct.left));
	mY = static_cast<float>(rand() % (rct.bottom - rct.top));
}

void Bug::Draw()
{
	mX += static_cast<float>(1 - rand() % 3);
	mY += static_cast<float>(1 - rand() % 3);

	Actor::Draw();
}
