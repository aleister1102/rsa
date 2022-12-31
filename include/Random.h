#pragma once
#include "BigInt.h"

class Random
{
public:
	BigInt next(uint32_t byteCount);
	BigInt next(BigInt n);
	BigInt next(BigInt a, BigInt b);
};

static Random random;
