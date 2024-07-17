#pragma once
#include "BaseTrout.h"

class ItemTrout :public BaseTrout
{
public:

	ItemTrout();
	~ItemTrout();

	void Initialize()override;

	void Update()override;

	void Draw()override;

private:
};