#pragma once
#include "BigInt.h"

BigInt gcd(BigInt a, BigInt b);

std::tuple<BigInt, BigInt, BigInt> extendedEuclidean(BigInt a, BigInt b);

BigInt inverseMod(BigInt a, BigInt m);

BigInt powMod(BigInt n, BigInt e, BigInt m);

bool millerRabinTest(BigInt n, BigInt d);