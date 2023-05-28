#pragma once
#include "D2DFramework.h"
#include "Actor.h"
#include <vector>

class ActorExample : public D2DFramework
{
	std::unique_ptr<Actor> mspBackground;
	std::vector<std::unique_ptr<Actor>> mspBug;

public:
	virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Actor Example", UINT width = 1024, UINT height = 768) override;
	
	virtual void Release() override;
	virtual void Render() override;
	
};

