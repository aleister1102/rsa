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

public:
	RSA(uint32_t byteCount);
	RSA(BigInt p, BigInt q);
	RSA(BigInt d);

private:
	void generatePrimes();
	void calculatePhiAndN();
	void generateEncryptionKey();
	void generateDecryptionKey();

public:
	static string encrypt(string plainText, BigInt n, BigInt e);
	static string decrypt(string cipherText, BigInt n, BigInt d);
	static void encryptFile(string p, BigInt n, BigInt e, string c);
	static void decryptFile(string c, BigInt n, BigInt d, string p);

public:
	static void test();
};
