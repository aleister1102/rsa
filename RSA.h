#pragma once
#include "BigInt.h"

using std::tuple;
using std::string;

class RSA
{
public:
	BigInt getD();

public:
	string encrypt(string plainText);
	string decrypt(string cipherText);
	void encryptFile(string p, string c);
	void decryptFile(string c, string p);
	void exportKeys(int format, int exportMethod);

public:
	RSA() {};
	RSA(uint32_t byteCount);
	RSA(const RSA& other) { *this = other; }
	RSA(BigInt n, BigInt e, BigInt d) : n(n), e(e), d(d) {};

public:
	BigInt n = 0;
	BigInt e = 0;
	uint32_t byteCount = 0;
	static inline int checkPrimeLoops = 5;

private:
	BigInt q = 0;
	BigInt p = 0;
	BigInt phi = 0;
	BigInt d = 0;

private:
	void generatePrimes();
	void calculateNandPhi();
	void generateEncryptionKey();
	void generateDecryptionKey();
	string getKeys();
	string getBinaryKeys();
};