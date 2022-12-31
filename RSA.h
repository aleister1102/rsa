#pragma once
#include "BigInt.h"

using std::cout;
using std::string;

class RSA
{
public:
	void exportKeys(int exportMethod);
	string encrypt(string plainText);
	string decrypt(string cipherText);
	void encryptFile(string p, string c);
	void decryptFile(string c, string p);

public:
	RSA() {}
	RSA(const RSA& other) { *this = other; }
	RSA(uint32_t byteCount, int base = BigIntBase::BASE_2);
	RSA(BigInt n, BigInt e, BigInt d, int base = BigIntBase::BASE_2) : n(n), e(e), d(d), base(base) {};

public:
	uint32_t byteCount = 0;
	int base = BigIntBase::BASE_2;
	static inline int checkPrimeLoops = 5;

public:
	BigInt n = 0;
	BigInt e = 0;

private:
	BigInt q = 0;
	BigInt p = 0;
	BigInt d = 0;
	BigInt phi = 0;

private:
	string getKeys();
	void generatePrimes();
	void calculateNandPhi();
	void generateEncryptionKey();
	void generateDecryptionKey();
};