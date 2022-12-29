#pragma once
#include "BigInt.h"

constexpr auto CHECK_PRIME_LOOPS = 10;

using std::tuple;

class RSA
{
private:
	BigInt q = 0;
	BigInt p = 0;
	BigInt phi = 0;
	BigInt d = 0;

public:
	BigInt n = 0;
	BigInt e = 0;
	uint32_t byteCount;

public:
	RSA(uint32_t byteCount);

private:
	void generatePrimes();
	void generateEncryptionKey();
	void generateDecryptionKey();
};
