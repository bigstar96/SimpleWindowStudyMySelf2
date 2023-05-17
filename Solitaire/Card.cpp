#include "Card.h"
#include <string>

namespace solitaire
{
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd },
		mIndex{ index },
		mType{ type },
		mX{ x },
		mY{ y },
		mIsFront{ false }
	{
		std::wstring filename;

		switch (mType)
		{
		case Type::Wolf:
			filename = L"File/card_creature_wolf.png";
			break;

		case Type::Dragon:
			filename = L"File/card_creature_dragon.png";
			break;

		case Type::Bear:
			filename = L"File/card_creature_bear.png";
			break;

		}

		mBack = std::make_unique<Gdiplus::Image>(L"File/card_back.png");
		mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
	}

	bool Card::CheckClicked(int x, int y)
	{
		/*if (x >= mX && y >= mY &&
			static_cast<UINT>(x) <= mX + mFront->GetWidth() &&
			static_cast<UINT>(y) <= mY + mFront->GetHeight())
		{
			Flip(!mIsFront);

			return true;
		}
		*/

		Gdiplus::Rect rct(mX, mY, mFront->GetWidth(), mFront->GetHeight());

		if (rct.Contains(x, y))
		{
			Flip(!mIsFront);

			return true;
		}
		return false;
	}

	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
		Invalidate();
	}

	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			graphics.DrawImage(
				mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight()
			);
		}
		else
		{
			graphics.DrawImage(
				mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight()
			);
		}
	}

	void Card::Invalidate()
	{
		RECT rct{
			mX, mY,
			static_cast<LONG>(mX + mBack->GetWidth()),
			static_cast<LONG>(mY + mBack->GetHeight())
		};
		InvalidateRect(mHwnd, &rct, false);
	}

}



