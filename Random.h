#pragma once
#include "BigInt.h"

class Random
{
private:
	static inline Random* instance = nullptr;

	Random()
	{
		srand(time(0));
	}

public:
	static Random* getInstance()
	{
		if (instance == nullptr)
			instance = new Random();

		return instance;
	}

public:
	BigInt next(uint32_t byteCount = 1);
	BigInt next(BigInt n);
	BigInt next(BigInt a, BigInt b);
};

static Random random = *Random::getInstance();
