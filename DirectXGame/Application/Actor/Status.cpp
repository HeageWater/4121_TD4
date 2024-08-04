#include "Status.h"
#include "MyMath.h"

void Status::Initialize()
{
	hp_ = 5;

	power_ = 1;
}

void Status::Update()
{

}

void Status::Random(size_t hp, size_t power)
{
	//ランダム
	hp_ = MyMath::GetRandom(0, hp) + 5;
	power_ = MyMath::GetRandom(0, power) + 1;
}
