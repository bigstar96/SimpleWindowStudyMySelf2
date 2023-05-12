#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>

namespace solitaire
{
	enum class Type
	{
		Wolf,
		Dragon,
		Bear
	};

	class Card
	{
	private:
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;

		HWND mHwnd;
		int mIndex;
		int mX;
		int mY;
		bool mIsFront;
		Type mType;

	public:
		Card(HWND hwnd, int index, Type type, int x, int y);

		bool CheckClicked(int x, int y);
		void Flip(bool isFront);
		void Draw(Gdiplus::Graphics& graphics);
		void Invalidate();

		Type GetType() { return mType; }
		int GetIndex() { return mIndex; }
	};
}