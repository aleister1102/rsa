#pragma once
#include "BigInt.h"

class BigIntRandom
{
private:
	static inline BigIntRandom* instance = nullptr;

	BigIntRandom()
	{
		srand(time(0));
	}

public:
	static BigIntRandom* getInstance()
	{
		if (instance == nullptr)
			instance = new BigIntRandom();

		return instance;
	}

public:
	BigInt next(uint32_t bitCount);
	BigInt next(BigInt n);
};
