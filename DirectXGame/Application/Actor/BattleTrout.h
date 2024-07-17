#pragma once
#include "BaseTrout.h"

class BattleTrout :public BaseTrout
{
public:

	BattleTrout();
	~BattleTrout();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
};