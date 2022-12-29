#pragma once
#include "BigInt.h"

constexpr auto CHECK_PRIME_LOOPS = 10;

using std::tuple;
using std::string;

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

private:
	void generatePrimes();
	void calculatePhiAndN();
	void generateEncryptionKey();
	void generateDecryptionKey();
	string encrypt(string plainText);

public:
	RSA(uint32_t byteCount);
	RSA(BigInt p, BigInt q);
	RSA(BigInt d);

public:
	void encryptFile(string p, BigInt e, string c);
};
