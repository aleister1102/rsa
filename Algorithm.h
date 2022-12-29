#pragma once
#include "BigInt.h"

class Algorithm
{
public:
	static BigInt gcd(BigInt a, BigInt b);
	static BigInt powMod(BigInt n, BigInt e, BigInt m);
	static bool isPrime(BigInt n, int k = 1);
	static bool millerRabinTest(BigInt n, BigInt d);
};
