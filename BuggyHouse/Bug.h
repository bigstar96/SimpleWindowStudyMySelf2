#pragma once
#include "Actor.h"



class Bug : public Actor
{
public:
	Bug(D2DFramework* pFramework);

	virtual void Draw() override;
};

