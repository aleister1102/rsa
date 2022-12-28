#pragma once
#include "BigInt.h"

class Algorithm
{
public:
	static BigInt gcd(BigInt a, BigInt b);
	static BigInt powMod(BigInt n, BigInt e, BigInt m);
};
