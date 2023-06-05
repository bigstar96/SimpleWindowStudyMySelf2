#pragma once
#include <list>
#include <memory>
#include <random>

#include "D2DFramework.h"
#include "Actor.h"

class BuggyHouse : public D2DFramework
{
	std::list<std::unique_ptr<Actor>> mBugList;
	std::unique_ptr<Actor> mspBackground;

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Buggy House",
		UINT width = 1024, UINT height = 768) override;

	virtual void Release() override;
	virtual void Render() override;

	void CheckBugs();
};

