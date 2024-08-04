#pragma once

class Status
{
public:
	void Initialize();

	void Update();

	void Random(size_t hp,size_t power);

	void SetHP(size_t hp) { hp_ = hp; };

	void SetPower(size_t power) { power_ = power; };

public:

	//
	size_t hp_;

	//
	size_t power_;

};